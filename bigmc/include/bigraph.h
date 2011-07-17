#ifndef _BIGRAPH_H
#define _BIGRAPH_H

// Bigraph is (V,E,ctrl,prnt,link) : <n,K> -> <m,L>
class bigraph {
	node *root;
	int nholes;
	int nregions;
	set<name> inner;
	set<name> outer;
	set<reactionrule *> rules;
public:
	static map<string,name> name_map;
	static map<string,control> control_map;
	static map<control,bool> activity_map;
	static map<control,int> arity_map;
	static control u_control;
	static name u_name;
	bigraph(int roots);
	~bigraph();
	node *get_root(int n);
	bigraph operator + (bigraph b);
	set<match *> find_matches();
	long hash();
	static control control_from_string(string n);
	static string control_to_string(control c);
	static name name_from_string(string n);
	static string name_to_string(name n);
	static bool activity(control c);
	static int arity(control c);
	static control add_control(string n, bool act, int ar);
	void add_outer_name(name n);
	void add_inner_name(name n);
	void set_root(node *n);
	void add_rule(reactionrule *r);
	bigraph *apply_match(match *m);
};


#endif
