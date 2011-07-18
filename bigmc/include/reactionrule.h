#ifndef _REACTIONRULE_H
#define _REACTIONRULE_H

class reactionrule {
public:
	term *redex;
	term *reactum;
	reactionrule(term *red, term *reac);
	~reactionrule();
};

#endif

