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
#ifndef _ANALYSER_H
#define _ANALYSER_H

class controlvisitor : public termvisitor {
public:
	controlvisitor();
	~controlvisitor();
	void visit(term *t);
	void visit(parallel *t);
	void visit(regions *t);
	void visit(prefix *t);
	void visit(hole *t);
	void visit(nil *t);
	set<control> controls;
	int nholes;
	int depth;
	int size;
};

// A delta can be invalid because it doesn't actually represent a lower bound on size change.
// For example, a.$0 -> a.($0 | $0) has size 2*sz($0), but we can't predict the size of $0.
struct delta_t {
	bool valid;
	int delta;

	string to_string() {
		std::stringstream out;
		if(valid) {
			out << delta;
		} else {
			return "<<unknown>>";
		}

		return out.str();
	}
};

struct ruleattr {
	reactionrule *rule;
	delta_t size_delta;
	delta_t depth_delta;
	set<control> precond;
	set<control> postcond;

	string to_string() {
		string cs = "";

		set<control> intersect;
		std::set_intersection( precond.begin(), precond.end(), postcond.begin(), postcond.end(), std::inserter( intersect, intersect.begin() ) );

		for(set<control>::iterator i = intersect.begin(); i!=intersect.end(); i++) {
			cs += " " + bigraph::control_to_string(*i);
		}

		string ds;
		set<control> diff1;
		std::set_difference( precond.begin(), precond.end(), postcond.begin(), postcond.end(), std::inserter( diff1, diff1.begin() ) );

		for(set<control>::iterator i = diff1.begin(); i!=diff1.end(); i++) {
			ds += " " + bigraph::control_to_string(*i);
		}
		
		string is;
		set<control> diff2;
		std::set_difference( postcond.begin(), postcond.end(), precond.begin(), precond.end(), std::inserter( diff2, diff2.begin() ) );

		for(set<control>::iterator i = diff2.begin(); i!=diff2.end(); i++) {
			is += " " + bigraph::control_to_string(*i);
		}

		string pc = "";

		for(set<control>::iterator i = precond.begin(); i!=precond.end(); i++) {
			pc += " " + bigraph::control_to_string(*i);
		}

		string psc = "";

		for(set<control>::iterator i = postcond.begin(); i!=postcond.end(); i++) {
			psc += " " + bigraph::control_to_string(*i);
		}


		return "Rule Attribute Analysis: " + rule->to_string() + "\n" +
			"  * Size delta: " + size_delta.to_string() + "\n" +
			"  * Depth delta: " + depth_delta.to_string() + "\n" +
			"  * Precondition: " + pc + "\n" +
			"  * Postcondition: " + psc + "\n" + 
			"  * Insertion set: " + is + "\n" +
			"  * Deletion set: " + ds + "\n" +
			"  * Constant set: " + cs;
	}
};

class analyser {
	bigraph *bg;
public:
	analyser(bigraph *b);
	~analyser();
	void interference();
	set<control> interference_set(set<control> s1, set<control> s2);
	static ruleattr *analyse(reactionrule *r);
	static bool orthogonalas(term *r1, term *r2);
	static bool orthogonal(term *r1, term *r2);
	static bool interferes(term *reactum, term *redex);
	
};

#endif
