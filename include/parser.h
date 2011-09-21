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
#ifndef _PARSER_H
#define _PARSER_H

class parser {
	FILE *fp;
	static term		*bg_mknode(prefixnode *p);
	static term		*bg_mknode(parallelnode *p);
	static term		*bg_mknode(holenode *p);
	static term		*bg_mknode(nilnode *p);
	static term		*bg_mknode(parsenode *p);
	static term		*bg_mknode(numnode *p);
	static term		*bg_mknode(controlnode *p);
	static term		*bg_mknode(regionnode *p);
	static control		bg_mkctrl(controlnode *p);
	static string		bg_mkname(namenode *p);
	static vector<name>	bg_names(seqnode *p);
	static vector<name>	bg_names(parsenode *p);
	static set<term *> 	bg_collapse(parallelnode *p); 
	static set<term *> 	bg_collapse(parsenode *p);
	static list<term *> 	bg_rcollapse(regionnode *p); 
	static list<term *> 	bg_rcollapse(parsenode *p); 
	static query		*mk_query(parsenode *p);
	static list<query_val *> mk_queryargs(parsenode *p);
	static query_val 	*mk_queryval(parsenode *p);

public:
	static void init(char *file);
	static void cleanup();
	static int  parse();
	static void add_result(parsenode *p);
	static bigraph *finish();
};

extern vector<parsenode *> g_declaration;
int parser_next_char();
void parser_add_result(parsenode *p);
void parser_import(char *module);
int yylex();
int yyparse();

#endif
