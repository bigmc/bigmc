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
	bool has_holes;
};

class analyser {
	bigraph *bg;
public:
	analyser(bigraph *b);
	~analyser();
	void interference();
	set<control> interference_set(set<control> s1, set<control> s2);
};

#endif
