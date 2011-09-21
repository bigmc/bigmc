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
#ifndef _MATCHER_H
#define _MATCHER_H

class matcher {
public:
	static set<match *> try_match(prefix *t, prefix *r, match *m);
	static set<match *> try_match(parallel *t, prefix *r, match *m);
	static set<match *> try_match(prefix *t, parallel *r, match *m);
	static set<match *> try_match(parallel *t, parallel *r, match *m);
	static set<match *> try_match(num *t, num *r, match *m);
	static set<match *> try_match(term *t, regions *r, match *m);
	static set<match *> try_match(term *t, hole *r, match *m);
	static set<match *> try_match(nil *t, term *r, match *m);
	static set<match *> try_match(term *t, term *r, match *m);

	static set<match *> region_match(term *t, list<term *> r, set<match *> m);

	static set<match *> try_match(term *t, reactionrule *r);
	static set<match *> try_match_anywhere(term *t, term *r, reactionrule *rl, match *m);
};

#endif
