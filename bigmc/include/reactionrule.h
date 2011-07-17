#ifndef _REACTIONRULE_H
#define _REACTIONRULE_H

class reactionrule {
public:
	node *redex;
	node *reactum;
	reactionrule(node *red, node *reac);
	~reactionrule();
};

#endif

