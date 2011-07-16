#ifndef _BIGRAPH_H
#define _BIGRAPH_H

// Bigraph is (V,E,ctrl,prnt,link) : <n,K> -> <m,L>
class bigraph {
	node *root;
	int nholes;
	int nregions;
	set<name> inner;
	set<name> outer;

public:
	bigraph(int roots);
	~bigraph();
	node get_root(int n);
	bigraph operator + (bigraph b);
	set<match> find_matches(bigraph redex);
	long hash();
	
};

struct control_map_entry {
	int arity;
	bool active;
};

extern map<control,control_map_entry> g_control_map;

#endif
