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
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <bigmc.h>
#include <stdio.h>

graph::graph(node *initial) {
	root = initial;

	if(initial != NULL)
		nodes.insert(initial);
}

graph::~graph() {
	set<node*>::iterator i = nodes.begin();

	while(i != nodes.end()) {
		delete *i;
		i++;
	}

	nodes.clear();
}

void graph::add(node *n) {
	nodes.insert(n);
	lut[n->hash] = n;
}

node *graph::get(unsigned long id) {
	map<unsigned long,node *>::iterator l = lut.find(id);

	if(l == lut.end()) return NULL;

	return l->second;
}

void graph::report() {
	cout << "Graph Report:" << endl;
	cout << "\tNumber of nodes: " << nodes.size() << endl;
}

unsigned long graph::size() {
	return nodes.size();
}

string graph::backtrace(node *n) {
	node *h = n;

	string bt = "";
	stringstream out;

	int i = 0;

	while(h != NULL) {
		string here = "";
		if(i == 0) here = "   <- *** VIOLATION ***";
		out << "#" << i << "  " << h->bg->get_root(0)->to_string() << here << endl;
			
		if(h->rule == NULL) {
			out << " >> " << "(root)" << endl;
		} else {
			out << " >> " << h->rule->to_string() << endl << endl;
		}


		i++;
		h = h->parent;
	}

	return out.str();
}

string graph::dump_dot() {
	if(global_cfg.graph_out == NULL) return "";

	stringstream out;

	out << "digraph reaction_graph {" << endl;
	out << "   rankdir=LR;" << endl;
	//string terminals = "LR_1 LR_2 LR_3";

	//out << "node [shape = doublecircle];" << terminals << ";" << endl;
	out << "   node [shape = circle];" << endl;

	stringstream coda;

	out << "   N_" << root->hash << " [shape=ellipse, color=lightblue2, style=filled, label=\"" << root->bg->get_root(0)->to_string() << "\"];" << endl;

	for(set<node *>::iterator i = nodes.begin(); i!=nodes.end(); i++) {
		string rr = "root";

		string dc = "shape = ellipse, ";

		if((*i)->terminal)
			dc = "shape = doubleoctagon, ";

		out << "N_" << (*i)->hash << " [ " << dc << "label=\"" << (*i)->bg->get_root(0)->to_string() << "\"];" << endl;

		if((*i)->rule != NULL)
			rr = (*i)->rule->to_string();

		if((*i)->parent != NULL)
			out << "   N_" << (*i)->parent->hash << " -> N_" << (*i)->hash << 
				" [ label = \"" << rr << "\" ];" << endl;
	}

	out << coda.str();
	out << "}" << endl;

	FILE *fp = fopen(global_cfg.graph_out, "w");
	if(!fp) {
		cerr << "Error: could not open graph file " << global_cfg.graph_out << " for writing\n";
		return "";
	}

	fprintf(fp, "%s\n", out.str().c_str());

	fclose(fp);

	return out.str();

	
}


string graph::dump_dot_forward() {
	if(global_cfg.graph_out == NULL) return "";

	stringstream out;

	out << "digraph reaction_graph {" << endl;
	out << "   rankdir=LR;" << endl;
	//string terminals = "LR_1 LR_2 LR_3";

	//out << "node [shape = doublecircle];" << terminals << ";" << endl;
	out << "   node [shape = none];" << endl;

	stringstream coda;

	out << "   N_" << root->hash << " [shape=rectangle, color=lightblue2, style=filled, label=\"" << root->bg->get_root(0)->to_string() << "\"];" << endl;

	for(set<node *>::iterator i = nodes.begin(); i!=nodes.end(); i++) {
		if(DEBUG)
			cout << "graph::dump: printing: " << (*i)->bg->to_string() << endl;
		string rr = "root";

		string dc = "shape = none, ";

		if((*i)->terminal)
			dc = "shape = doubleoctagon, color=darkolivegreen3, style=filled, ";

		out << "N_" << (*i)->hash << " [ " << dc << "label=\"" << (*i)->bg->get_root(0)->to_string() << "\"];" << endl;

		for(set<pair<node *, reactionrule *> >::iterator j = (*i)->target.begin();
			j != (*i)->target.end(); j++) {

			if(DEBUG)
				cout << "graph::dump: node printing: " << j->second << " & " << 
					j->first->bg->to_string() << endl;

			rr = "?";
			
			if(j->second != NULL)
				rr = j->second->to_string();

			if(j->first != NULL)
				out << "   N_" << (*i)->hash << " -> N_" << j->first->hash << 
					" [ label = \"" << rr << "\" ];" << endl;

		}

	}

	out << coda.str();
	out << "}" << endl;

	FILE *fp = fopen(global_cfg.graph_out, "w");
	if(!fp) {
		cerr << "Error: could not open graph file " << global_cfg.graph_out << " for writing\n";
		return "";
	}

	fprintf(fp, "%s\n", out.str().c_str());

	fclose(fp);

	return out.str();

	
}
