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
using namespace std;
#include <iostream>
#include <string>
#include <stdio.h>
#include <assert.h>

#include <bigmc.h>
#include "bgparser.hpp"

#ifndef _WIN32
#include <unistd.h> /* for isatty(3) */
#endif /* !_WIN32 */

#ifdef HAVE_LIBREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#else

#include <stdlib.h>
#include <string.h>

char *readline(char *prmpt) {
	size_t size = 0;
	size_t len  = 0;
	size_t last = 0;
	char * buf  = NULL;

	printf("%s",prmpt);

	do {
		size += 1024; 
		buf = (char *)realloc(buf,size);
		fgets(buf+last,size,stdin);
		len = strlen(buf);
		last = len - 1;
	} while (!feof(stdin) && buf[last]!='\n');
	return buf;
}

void using_history() {
	;
}

void add_history(char *s) {
	;
}

#endif

static char *g_inputbuffer = (char *)NULL;
int g_inputbuffer_len = 0;
int g_inputbuffer_offset = 0;
FILE *g_fp = NULL;
bool g_userbreak = false;
vector<parsenode *> g_parsetree;
bool g_has_check = false;

void parser_add_result(parsenode *p) {
	if(DEBUG) cout << "BUG: parser_add_result: " << p->to_string() << endl;
	g_parsetree.push_back(p);
}

void parser_import(char *module) {
	if(strcmp(module, "calculation") == 0) {
		global_cfg.calculation = true;
	}
}

void check_warn() {
	fprintf(stderr, "Error: your model file doesn't have a '%%check' directive, so it won't do anything! Exiting...\n");
}

int parser_next_char() {
	if(g_inputbuffer_len > g_inputbuffer_offset && 
		g_inputbuffer[g_inputbuffer_offset] != EOF &&
		g_inputbuffer[g_inputbuffer_offset] != '\0') {
		return g_inputbuffer[g_inputbuffer_offset++];
	}

	// We're at the end of the buffer -- it needs a refill!

	g_inputbuffer_len = 0;
	g_inputbuffer_offset = 0;

	if(!g_fp) {
		// We are in readline interactive mode
  		
		/* If the buffer has already been allocated, return the memory to the free pool. */
		if (g_inputbuffer) {
			free (g_inputbuffer);
			g_inputbuffer = (char *)NULL;
		}

		/* Get a line from the user. */
		g_inputbuffer = readline ("bigmc> ");

		/* If the line has any text in it, save it on the history. */
		if (g_inputbuffer && *g_inputbuffer)
			add_history (g_inputbuffer);

		if(g_inputbuffer == NULL) {
			printf("\n");
			exit(0);
		}

		g_inputbuffer_len = strlen(g_inputbuffer);
		g_inputbuffer_offset = 0;
	} else {
		// We are in file mode
		if(g_inputbuffer) {
			free(g_inputbuffer);
			g_inputbuffer = (char *)NULL;
		}

		g_inputbuffer = (char *)malloc(8192);
		if(!g_inputbuffer) {
			perror("Error allocating memory while reading input file");
			exit(1);
			return EOF;
		}

		if(fgets(g_inputbuffer, 8192, g_fp) == NULL) {
			g_userbreak = true;
			if(!g_has_check) check_warn();
			exit(1);
		}

		g_inputbuffer_len = strlen(g_inputbuffer);
		g_inputbuffer_offset = 0;
	}

	if(g_inputbuffer_len == 0) return '\n';
	if(*g_inputbuffer == '\0' || *g_inputbuffer == EOF) {
		exit(0);
	}

	if(strncmp("exit",g_inputbuffer,4) == 0 || strncmp("quit",g_inputbuffer,4) == 0) {
		exit(0);
	}

	if(strncmp("check",g_inputbuffer,5) == 0 || strncmp("check;",g_inputbuffer,6) == 0 ||
	   strncmp("%check",g_inputbuffer,6) == 0 || strncmp("%check;",g_inputbuffer,7) == 0) {
		return EOF;
		
		//g_parsetree.clear(); // TODO free memory here!
	}

	return g_inputbuffer[g_inputbuffer_offset++];
}

void parser::init(char *file) {
	bool tty;
#ifndef _WIN32
	tty = isatty(fileno(stdin));
#else
	tty = true;
#endif
	if (tty && file == NULL) {
		print_version();
		cout << "Hint: remember to enter 'check' once you have finished entering your model!" << endl;
		g_fp = NULL;
		using_history();
	} else {
		if (file != NULL && strcmp(file, "-") != 0) {
			g_fp = fopen(file,"r");
			if(g_fp == NULL) {
				perror("Cannot open model file");
				exit(1);
			}
		} else g_fp = stdin;
	}
}

void parser::cleanup() {
	if(g_fp)
		fclose(g_fp);
}

int parser::parse() {
	yyparse();

	if(DEBUG) cout << "Beginning check: " << g_parsetree.size() << endl;

	bigraph *b = parser::finish();
	g_has_check = true;
	driver::check(b);

	return 0;
}

void add_result(parsenode *p) {
	g_parsetree.push_back(p);
} 

term *parser::bg_mknode(prefixnode *p) {
	if(DEBUG) fprintf(stderr, "BUG: parser::bg_mknode(prefixnode): ");
	assert(p != NULL);

	if(p->prefix->type == NODE_NUM) {
		return new num(((numnode*)p->prefix)->data);
	}

	controlnode *cn = dynamic_cast<controlnode *>(p->prefix);
	assert(cn != NULL);

	return new prefix(bigraph::control_from_string(cn->name->to_string()), 
			(cn->links == NULL) ? vector<name>() : bg_names(cn->links), 
			bg_mknode(p->suffix));
}

set<term *> parser::bg_collapse(parallelnode *p) {
	set<term *> l = bg_collapse(p->lhs);
	set<term *> r = bg_collapse(p->rhs);
	r.insert(l.begin(),l.end());
	return r;
}

set<term *> parser::bg_collapse(parsenode *p) {
	if(p->type == NODE_PARALLEL)
		return bg_collapse((parallelnode *)p);

	set<term *> s;

	if(p->type != NODE_NIL)
		s.insert(bg_mknode(p));
	return s; 
}

list<term *> parser::bg_rcollapse(regionnode *p) {
	list<term *> l = bg_rcollapse(p->lhs);
	list<term *> r = bg_rcollapse(p->rhs);
	l.insert(l.end(), r.begin(),r.end());
	return l;
}

list<term *> parser::bg_rcollapse(parsenode *p) {
	if(p->type == NODE_REGION)
		return bg_rcollapse((regionnode *)p);

	list<term *> s;
	s.push_back(bg_mknode(p));
	return s; 
}

term *parser::bg_mknode(parallelnode *p) {
	if(DEBUG) fprintf(stderr, "BUG: parser::bg_mknode(parallelnode): ");

	if(p == NULL) return new nil();

	if(DEBUG) cerr << p->to_string() << endl;

	return new parallel(bg_collapse(p));
}

term *parser::bg_mknode(regionnode *p) {
	if(DEBUG) fprintf(stderr, "BUG: parser::bg_mknode(regionnode): ");

	if(p == NULL) return new nil();

	if(DEBUG) cerr << p->to_string() << endl;

	return new regions(bg_rcollapse(p));
}

term *parser::bg_mknode(holenode *p) {
	if(DEBUG) fprintf(stderr, "BUG: parser::bg_mknode(holenode): ");

	if(p == NULL) return new nil();

	if(DEBUG) cerr << p->to_string() << endl;
	
	return new hole(p->n);
}

term *parser::bg_mknode(nilnode *p) {
	if(DEBUG) fprintf(stderr, "BUG: parser::bg_mknode(nilnode): ");

	return new nil();
}


term *parser::bg_mknode(numnode *p) {
	if(DEBUG) fprintf(stderr, "BUG: parser::bg_mknode(numnode): ");

	return new num(p->data);
}

vector<name> parser::bg_names(seqnode *p) {
	namenode *l = (namenode *)p->lhs;
	string n = l->to_string();
	name nm = bigraph::name_from_string(n);
	vector<name> v;
	v.push_back(nm);

	vector<name> w = bg_names(p->rhs);

	for(unsigned int i = 0; i < w.size(); i++) {
		v.push_back(w[i]);
	}

	return v;
}

vector<name> parser::bg_names(parsenode *p) {
	if(!p) return vector<name>();

	switch(p->type) {
		case NODE_NAME: {
			vector<name> v;
			name nm = bigraph::name_from_string(((namenode *)p)->to_string());
			v.push_back(nm);
			return v;
		}
		case NODE_SEQ: {
			return bg_names((seqnode *) p);
		}
		default:
			cerr << "BUG: Invalid name: " << p->to_string() << endl;
			exit(1);
			break;
	}

	return vector<name>();
}

term *parser::bg_mknode(controlnode *p) {
	if(p == NULL) return new nil();
	
	prefixnode *n = new prefixnode(p,NULL);

	return bg_mknode(n);
}

term *parser::bg_mknode(parsenode *p) {
	if(p == NULL) return new nil();
	
	if(DEBUG) cout << "parser::bg_mknode(): " << p << endl;

	switch(p->type) {
	case NODE_PREFIX: {
		if(DEBUG) cout << "parser::bg_mknode(): NODE_PREFIX: " << p << endl;
		prefixnode *pp = static_cast<prefixnode *>(p);
		assert(pp != NULL);
		return bg_mknode(pp);
		break;
	}
	case NODE_PARALLEL:
		return bg_mknode((parallelnode *) p);
		break;
	case NODE_HOLE:
		return bg_mknode((holenode *) p);
		break;
	case NODE_NIL:
		return bg_mknode((nilnode *) p);
		break;
	case NODE_REGION:
		return bg_mknode((regionnode *) p);
		break;
	case NODE_CONTROL: {
		if(DEBUG) cout << "parser::bg_mknode(): NODE_CONTROL: " << p << endl;
		controlnode *pp = dynamic_cast<controlnode *>(p);
		assert(pp != NULL);
		return bg_mknode(pp);
		break;
	}
	case NODE_NUM:
		return bg_mknode((numnode *) p);
		break;
	default:
		fprintf(stderr, "Malformed term structure: ");
		cerr << p->to_string() << endl;
		exit(1);
		break;
	}

	return NULL;
}

query_val *parser::mk_queryval(parsenode *p) {
	if(p == NULL) {
		rerror("parser::mk_queryval") << "Invalid value in property term" << endl;
		exit(1);
	}

	switch(p->type) {
	case NODE_NUM:
		return new query_num(((numnode *)p)->data);
	case NODE_CONTROL: case NODE_PREFIX: case NODE_PARALLEL: case NODE_HOLE:
		return new query_term(bg_mknode(p));
	case NODE_NAME:
		return new query_id(((namenode *)p)->nme);
	}
}

list<query_val *> parser::mk_queryargs(parsenode *p) {
	list<query_val *> vals;

	if(p == NULL) return vals;

	while(p->type == NODE_SEQ) {
		vals.push_back(mk_queryval(((seqnode *)p)->lhs));
		p = ((seqnode *)p)->rhs;
	}

	vals.push_back(mk_queryval(p));

	return vals;
}

query *parser::mk_query(parsenode *p) {
	switch(p->type) {
		case NODE_BIN: {
			binnode *pp = (binnode *)p;
			return new query_bin(mk_query(pp->lprop),pp->oper,mk_query(pp->rprop));
		} case NODE_NOT: {
			notnode *pp = (notnode *)p;
			query_not *a = new query_not(mk_query(pp->prop));
			return a;
		}
		case NODE_PRED: {
			prednode *pp = (prednode *)p;
			if(pp->prop != NULL)
				return new query_predicate(pp->name, mk_queryargs(pp->prop));
			else
				return new query_predicate(pp->name, list<query_val*>());
		}
		case NODE_NUM: {
			return mk_queryval(p);
		}
		case NODE_QUERY: {
			querynode *pp = (querynode *)p;
			return new query_scope(pp->name, mk_query(pp->prop));
		}
		default:
			cerr << "parser::mk_query(): Invalid syntax in query: " << p->to_string() << endl;
			exit(1);
			break;
	}

	return NULL;
}

bigraph *parser::finish() {
	if(DEBUG) fprintf(stderr, "BUG: parser::finish()\n");
	
	bigraph *b = new bigraph(1);
	
	vector<parsenode *>::reverse_iterator it;
	for(it=g_parsetree.rbegin(); it!=g_parsetree.rend(); ++it) {
		switch((*it)->type) {
			case NODE_INTERFACE: {
				if(DEBUG) printf("NODE_INTERFACE\n");
				interfacenode *t = (interfacenode *)(*it);
				string n = ((namenode *)t->name)->to_string();
				name nm = b->name_from_string(n);
				if(t->outer)
					b->add_outer_name(nm);
				else
					b->add_inner_name(nm);
				break;
			}
			case NODE_SIGNATURE: {
				signaturenode *t = (signaturenode *)(*it);
				string n = ((namenode *)t->name)->to_string();
				control c = bigraph::add_control(n,t->active,t->arity);
				if(DEBUG) printf("NODE_SIGNATURE: %s (%d) : %d\n", n.c_str(), c, t->arity);
				break;
			}
			case NODE_REGION: case NODE_PREFIX: case NODE_PARALLEL: 
			case NODE_HOLE: case NODE_CONTROL: case NODE_NUM: {
				b->set_root(bg_mknode(*it));
				break;
			}
			case NODE_REACTION: {
				if(DEBUG) printf("NODE_REACTION\n");
				reactionnode *t = (reactionnode *)(*it);

				reactionrule *rr = new reactionrule(bg_mknode(t->redex),bg_mknode(t->reactum));

				rr->set_name(t->name);

				b->add_rule(rr);

				break;
			}
			case NODE_PROPERTY: {
				if(DEBUG) printf("NODE_PROPERTY\n");
				propertynode *p = (propertynode *)(*it);
				query *q = mk_query(p->prop);
				mc::add_property(p->name,q);
				break;
			}
			default:
				fprintf(stderr, "BUG: Fell through in parser::finish():\n");
				cerr << (*it)->to_string() << endl;
				break;
		}

		delete *it;
	}

	return b;
}

// Some flex/bison specific functions

//void yyerror (const char *error) {
//	fprintf(stderr, "%d: %s at %s\n", yylineno, error, yytext);
//}

void yyerror(const char *s)
{
  if(yylloc.first_line)
    fprintf(stderr, "%d.%d-%d.%d: error: %s\n", yylloc.first_line, yylloc.first_column,
	    yylloc.last_line, yylloc.last_column, s);

}


