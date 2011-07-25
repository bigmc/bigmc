using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <bigmc.h>

graph::graph(node *initial) {
	root = initial;

	if(initial != NULL)
		nodes.insert(initial);
}

graph::~graph() {

}

void graph::add(node *n) {
	nodes.insert(n);
	lut[n->hash] = n;
}

node *graph::get(unsigned long id) {
	return lut[id];
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

	out << "   N_" << root->hash << " [shape=doublecircle, color=lightblue2, style=filled, label=\"" << root->bg->get_root(0)->to_string() << "\"];" << endl;

	for(set<node *>::iterator i = nodes.begin(); i!=nodes.end(); i++) {
		string rr = "root";

		string dc = "";

		if((*i)->terminal)
			dc = "shape = doublecircle, ";

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
	out << "   node [shape = circle];" << endl;

	stringstream coda;

	out << "   N_" << root->hash << " [shape=doublecircle, color=lightblue2, style=filled, label=\"" << root->bg->get_root(0)->to_string() << "\"];" << endl;

	for(set<node *>::iterator i = nodes.begin(); i!=nodes.end(); i++) {
		string rr = "root";

		string dc = "";

		if((*i)->terminal)
			dc = "shape = doublecircle, color=darkolivegreen3, style=filled, ";

		out << "N_" << (*i)->hash << " [ " << dc << "label=\"" << (*i)->bg->get_root(0)->to_string() << "\"];" << endl;

		for(set<pair<node *, reactionrule *> >::iterator j = (*i)->target.begin();
			j != (*i)->target.end(); j++) {
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
