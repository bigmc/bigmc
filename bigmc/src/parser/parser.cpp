using namespace std;
#include <iostream>
#include <string>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <assert.h>
#include <bigmc.h>
#include "bgparser.tab.hpp"

static char *g_inputbuffer = (char *)NULL;
int g_inputbuffer_len = 0;
int g_inputbuffer_offset = 0;
FILE *g_fp = NULL;
bool g_userbreak = false;
vector<parsenode *> g_parsetree;
bool g_has_check = false;




void parser_add_result(parsenode *p) {
	if(g_debug) cout << "BUG: parser_add_result: " << p->to_string() << endl;
	g_parsetree.push_back(p);
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
	if(file == NULL) {
		print_version();
		cout << "Hint: remember to enter 'check' once you have finished entering your model!" << endl;
		g_fp = NULL;
		using_history();
	} else {
		g_fp = fopen(file,"r");
		if(g_fp == NULL) {
			perror("Cannot open model file");
			exit(1);
		}
	}
}

void parser::cleanup() {
	if(g_fp)
		fclose(g_fp);
}

int parser::parse() {
	yyparse();

	if(g_debug) cout << "Beginning check: " << g_parsetree.size() << endl;

	bigraph *b = parser::finish();
	g_has_check = true;
	driver::check(b);

	return 0;
}

void add_result(parsenode *p) {
	g_parsetree.push_back(p);
} 

term *parser::bg_mknode(prefixnode *p) {
	if(g_debug) fprintf(stderr, "BUG: parser::bg_mknode(prefixnode): ");
	assert(p != NULL);

	controlnode *cn = dynamic_cast<controlnode *>(p->prefix);
	assert(cn != NULL);

	return new prefix(bigraph::control_from_string(cn->name->to_string()), 
			(cn->links == NULL) ? vector<name>() : bg_names(cn->links), 
			bg_mknode(p->suffix));
}

set<term *> parser::bg_collapse(parallelnode *p) {
	set<term *> l = bg_collapse(p->lhs);
	set<term *> r = bg_collapse(p->rhs);
	l.insert(r.begin(),r.end());
	return l;
}

set<term *> parser::bg_collapse(parsenode *p) {
	if(p->type == TPAR)
		return bg_collapse((parallelnode *)p);

	set<term *> s;
	s.insert(bg_mknode(p));
	return s; 
}

term *parser::bg_mknode(parallelnode *p) {
	if(g_debug) fprintf(stderr, "BUG: parser::bg_mknode(parallelnode): ");

	if(p == NULL) return new nil();

	if(g_debug) cerr << p->to_string() << endl;

	return new parallel(bg_collapse(p));
}

term *parser::bg_mknode(holenode *p) {
	if(g_debug) fprintf(stderr, "BUG: parser::bg_mknode(holenode): ");

	if(p == NULL) return new nil();

	if(g_debug) cerr << p->to_string() << endl;
	
	return new hole(p->n);
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
	
	if(g_debug) cout << "parser::bg_mknode(): " << p << endl;

	switch(p->type) {
	case NODE_PREFIX: {
		if(g_debug) cout << "parser::bg_mknode(): NODE_PREFIX: " << p << endl;
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
	case NODE_CONTROL: {
		if(g_debug) cout << "parser::bg_mknode(): NODE_CONTROL: " << p << endl;
		controlnode *pp = dynamic_cast<controlnode *>(p);
		assert(pp != NULL);
		return bg_mknode(pp);
		break;
	}
	default:
		fprintf(stderr, "Malformed term structure: ");
		cerr << p->to_string() << endl;
		exit(1);
		break;
	}

	return NULL;
}

query *parser::mk_query(parsenode *p) {
	switch(p->type) {
		case NODE_AND: {
			andnode *pp = (andnode *)p;
			query_and *a = new query_and(mk_query(pp->lprop),mk_query(pp->rprop));
			return a;
		}
		case NODE_OR: {
			ornode *pp = (ornode *)p;
			query_or *a = new query_or(mk_query(pp->lprop),mk_query(pp->rprop));
			return a;
		}
		case NODE_NOT: {
			notnode *pp = (notnode *)p;
			query_not *a = new query_not(mk_query(pp->prop));
			return a;
		}
		case NODE_PRED: {
			prednode *pp = (prednode *)p;
			if(pp->prop != NULL)
				return new query_predicate(pp->name, bg_mknode(pp->prop));
			else
				return new query_predicate(pp->name, NULL);
		}
		default:
			cerr << "parser::mk_query(): Invalid syntax in query: " << p->to_string() << endl;
			exit(1);
			break;
	}

	return NULL;
}

bigraph *parser::finish() {
	if(g_debug) fprintf(stderr, "BUG: parser::finish()\n");
	
	bigraph *b = new bigraph(1);
	
	vector<parsenode *>::reverse_iterator it;
	for(it=g_parsetree.rbegin(); it!=g_parsetree.rend(); ++it) {
		switch((*it)->type) {
			case NODE_INTERFACE: {
				if(g_debug) printf("NODE_INTERFACE\n");
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
				if(g_debug) printf("NODE_SIGNATURE: %s (%d) : %d\n", n.c_str(), c, t->arity);
				break;
			}
			case NODE_PREFIX: case NODE_PARALLEL: case NODE_HOLE: case NODE_CONTROL: {
				b->set_root(bg_mknode(*it));
				break;
			}
			case NODE_REACTION: {
				if(g_debug) printf("NODE_REACTION\n");
				reactionnode *t = (reactionnode *)(*it);

				b->add_rule(
					new reactionrule(bg_mknode(t->redex),bg_mknode(t->reactum))
				);

				break;
			}
			case NODE_PROPERTY: {
				if(g_debug) printf("NODE_PROPERTY\n");
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


