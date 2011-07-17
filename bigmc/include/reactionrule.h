#ifndef _REACTIONRULE_H
#define _REACTIONRULE_H

class reactionrule {
	node *redex;
	node *reactum;
public:
	reactionrule(node *red, node *reac);
	~reactionrule();
};

#endif

