/*******************************************************************************
*
* Copyright (C) 2011 Gian Perrone (http://itu.dk/~gdpe)
* 
* BigMC - A bigraphical model checker (http://bigraph.org/bigmc).
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
* USA.
*********************************************************************************/
using namespace std;
#include <string>
#include <set>
#include <map>
#include <iostream>

#include <bigmc.h>

#ifdef HAVE_PTHREAD
#include <pthread.h>

pthread_mutex_t mcmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wqmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gmutex = PTHREAD_MUTEX_INITIALIZER;
#endif

map<string,query*> mc::properties;

mc::mc(bigraph *b) {
	node *n = new node(b,NULL,NULL);
	g = new graph(n);
	workqueue.push_back(n);
	steps = 0;
	cout << "mc::mc: preorder: " << subtree::preorder_string(b->get_root(0)) << endl;
}

mc::~mc() {

}

void *mc::thread_wrapper( void *i ) {
	mc *nc = reinterpret_cast<mc*>(i);

	while(nc->step())
		;

	return NULL;
}

bool mc::check() {
	#ifdef HAVE_PTHREAD

	if(global_cfg.threads <= 1) {
		thread_wrapper(this);
		cout << "mc::check(): Complete!" << endl;
		cout << report(steps) << endl;

		return false;
	}

	pthread_t *threads = new pthread_t[global_cfg.threads];
	for(int i = 0; i<global_cfg.threads; i++) {
		rinfo("mc::check") << "Worker thread #" << i << " started" << endl;
		pthread_create(&threads[i], NULL, &mc::thread_wrapper, this);
	}

	for(int i = 0; i<global_cfg.threads; i++) {
		pthread_join(threads[i], NULL);
		rinfo("mc::check") << "Worker thread #" << i << " finished" << endl;
	}

	cout << "mc::check(): Complete!" << endl;
	cout << report(steps) << endl;

	#else
	thread_wrapper(this);
	#endif

	return false;
}

bool mc::check_properties(node *n) {
	if(n->is_visited()) return true;

	for(map<string,query*>::iterator i = properties.begin(); i!=properties.end(); i++) {
		if(!i->second->check(n)) {
			// Failed
			cout << "*** Found violation of property: " << i->first << endl;
			cout << "*** " << i->first << ": " << i->second->to_string() << endl;
			if(!global_cfg.check_local) cout << g->backtrace(n);
			else cout << "[Backtrace unavailable in local checking mode]" << endl;
			return false;
		}
	}

	n->set_visited(true);
	return true;
}

string mc::report(int step) {
	stringstream out;
	rinfo("mc::report") << "[q: " << workqueue.size() << " / g: " << g->size() << "] @ " << step;
	g->dump_dot_forward();
	return out.str();
}

// returns true while there is work to do
bool mc::step() {
	if(steps >= global_cfg.max_steps) {
		cout << "mc::step(): Interrupted!  Reached maximum steps: " << global_cfg.max_steps << endl;
		cout << report(steps) << endl;
		return false;
	}

	#ifdef HAVE_PTHREAD
	pthread_mutex_lock( &mcmutex );
	#endif
	
	steps++;
	int step = steps;

	if(workqueue.size() == 0) {
		// We're done!

		#ifdef HAVE_PTHREAD

		cout << "workqueue.size() == 0, exiting thread..." << endl;
		pthread_mutex_unlock( &mcmutex );
		return false;

		#else
		cout << "mc::step(): Complete!" << endl;
		cout << report(step) << endl;
		// TODO: sound the alarms and release the balloons at this point.

		exit(0);
		return false; 
		#endif
	}

	node *n = workqueue.front();
	workqueue.pop_front();

	if(global_cfg.print_mode) {
		cout << step << ": " << n->bg->get_root(0)->to_string() << endl;
	}

	#ifdef HAVE_PTHREAD
	pthread_mutex_unlock( &mcmutex );
	#endif

	bigraph *b = n->bg;

	set<match *> matches = b->find_matches();
	if(matches.size() == 0) {
		n->terminal = true;

		return true;
	}

	if(global_cfg.check_local) {
		#ifdef HAVE_PTHREAD
		pthread_mutex_lock( &mcmutex );
		#endif

		checked[n->hash] = true;
		
		#ifdef HAVE_PTHREAD
		pthread_mutex_unlock( &mcmutex );
		#endif
	}

	for(set<match *>::iterator it = matches.begin(); it != matches.end(); ++it) {
		bigraph *b2 = b->apply_match(*it);
		node *n2 = new node(b2,n,(*it)->get_rule());

		if(!global_cfg.check_local) {
			node *n3 = g->get(n2->hash);
			if(n3 != NULL) {
				delete n2;
				n2 = n3;

				#ifdef HAVE_PTHREAD
				pthread_mutex_lock( &mcmutex );
				#endif

				n->add_target(n2,(*it)->get_rule());
	
				#ifdef HAVE_PTHREAD
				pthread_mutex_unlock( &mcmutex );
				#endif
			} else {
				
				#ifdef HAVE_PTHREAD
				pthread_mutex_lock( &mcmutex );
				#endif
			
				workqueue.push_back(n2);

				g->add(n2);
				n->add_target(n2,(*it)->get_rule());
	
				#ifdef HAVE_PTHREAD
				pthread_mutex_unlock( &mcmutex );
				#endif
			}
			
		} else {
			if(checked[n2->hash]) {
				// We've already checked this one!
				delete n2;
			} else {
				#ifdef HAVE_PTHREAD
				pthread_mutex_lock( &mcmutex );
				#endif

				workqueue.push_back(n2);

				#ifdef HAVE_PTHREAD
				pthread_mutex_unlock( &mcmutex );
				#endif
			}
		}

		if(DEBUG) cout << "BUG: mc::step(): new node" << endl << b2->to_string() << endl;	
		if(DEBUG) cout << "BUG: mc::step(): workq size: " << workqueue.size() << endl;
	}

	if(global_cfg.report_interval > 0 && step % global_cfg.report_interval == 0) {
		cout << report(step) << endl;
	}

	
	if(!check_properties(n)) {
		cout << "mc::step(): Counter-example found." << endl;
		return false;
	}

	if(global_cfg.check_local)
		delete n;

	return true;
}

void mc::add_property(string s,query *q) {
	properties[s] = q;
}
