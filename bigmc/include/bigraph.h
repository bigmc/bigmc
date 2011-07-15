#ifndef _BIGRAPH_H
#define _BIGRAPH_H

// Bigraph is (V,E,ctrl,prnt,link) : <n,K> -> <m,L>
class bigraph {
	node *root;
	set<edge> e;
	int nholes;
	int nregions;
	set<string> inner;
	set<string> outer;

public:
	bigraph(int roots);
	~bigraph();
	node get_root(int n);
	bigraph operator + (bigraph b);
	set<match> find_matches(bigraph redex);
	long hash();
	
};

#endif
