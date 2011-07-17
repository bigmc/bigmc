using namespace std;
#include <iostream>
#include <string>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <bigmc.h>

static char *g_inputbuffer = (char *)NULL;
int g_inputbuffer_len = 0;
int g_inputbuffer_offset = 0;
FILE *g_fp = NULL;
bool g_userbreak = false;
vector<parsenode *> g_parsetree;
bool g_has_check = false;

#define YY_INPUT(buf, result, max_size)        \
{                                              \
	int yyc = parser_next_char();                    \
	result = (EOF == yyc) ? 0 : (*(buf)= yyc, 1); \
}


void parser_add_result(parsenode *p) {
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

	if(strncmp("%check",g_inputbuffer,6) == 0 || strncmp("%check;",g_inputbuffer,7) == 0) {
		bigraph *b = parser::finish();
		g_has_check = true;
		driver::check(b);

		cout << "Would start model checking here!" << endl;

		for(vector<parsenode *>::iterator it=g_parsetree.begin();it!=g_parsetree.end();++it) {
			cout << (*it)->to_string() << endl;
		} 

		g_parsetree.empty(); // TODO free memory here!

		exit(0);
	}

	return g_inputbuffer[g_inputbuffer_offset++];
}

void parser::init(char *file) {
	if(file == NULL) {
		print_version();
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
	while(!g_userbreak && yyparse()) {
	}

	return 0;
}

void add_result(parsenode *p) {
	g_parsetree.push_back(p);
} 

node *parser::bg_mknode(prefixnode *p) {
	fprintf(stderr, "BUG: parser::bg_mknode(prefixnode): ");
	cerr << p->to_string() << endl;
	node *n1 = bg_mknode(p->prefix);
	// FIXME: This deals only with 1 child!
	vector<node *> n2 = bg_collapse(p->suffix);
	
	for(vector<node *>::iterator it = n2.begin(); it != n2.end(); ++it) {
		n1->add(*it);
	}
	
	return n1;
}

node *parser::bg_mknode(parallelnode *p) {
	fprintf(stderr, "BUG: parser::bg_mknode(parallelnode): ");
	cerr << p->to_string() << endl;

	exit(1);	
	return NULL;
}

node *parser::bg_mknode(holenode *p) {
	fprintf(stderr, "BUG: parser::bg_mknode(holenode): ");
	cerr << p->to_string() << endl;
	hole *h = new hole(p->n);
	
	return h;
}

vector<node *> parser::bg_collapse(parallelnode *p) {
	vector<node *> v = bg_collapse(p->lhs);
	vector<node *> w = bg_collapse(p->rhs);
	cout << "bg_collapse: set" << endl;

	for(unsigned int i = 0; i < w.size(); i++) {
		cout << "collapse merge: " << w[i]->to_string() << endl;
		v.push_back(w[i]);
	}

	return v;
}

vector<node *> parser::bg_collapse(parsenode *p) {
	if(p->type == NODE_PARALLEL)
		return bg_collapse((parallelnode *)p);

	vector<node *> v;
	cout << "bg_collapse: singleton" << endl;
	v.push_back(bg_mknode(p));
	return v;
}

node *parser::bg_mknode(controlnode *p) {
	fprintf(stderr, "BUG: parser::bg_mknode(controlnode): ");
	cerr << p->to_string() << endl;

	string nm = ((namenode *)(p->name))->to_string();
	control c = bigraph::control_from_string(nm);
	node *n = new node(c);

	return n;
}

node *parser::bg_mknode(parsenode *p) {
	if(p == NULL) {
		fprintf(stderr, "BUG: parser::bg_mknode(parsenode): %s\n", "NULL");
		exit(1);
	}

	switch(p->type) {
	case NODE_PREFIX:
		return bg_mknode((prefixnode *) p);
		break;
	case NODE_PARALLEL:
		return bg_mknode((parallelnode *) p);
		break;
	case NODE_HOLE:
		return bg_mknode((holenode *) p);
		break;
	case NODE_CONTROL:
		return bg_mknode((controlnode *) p);
		break;
	default:
		fprintf(stderr, "Malformed node structure: ");
		cerr << p->to_string() << endl;
		break;
	}

	return NULL;
}

bigraph *parser::finish() {
	fprintf(stderr, "BUG: parser::finish()\n");
	
	bigraph *b = new bigraph(1);
	
	vector<parsenode *>::iterator it;
	for(it=g_parsetree.begin(); it!=g_parsetree.end(); ++it) {
		switch((*it)->type) {
			case NODE_INTERFACE: {
				printf("NODE_INTERFACE\n");
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
				printf("NODE_SIGNATURE: %d\n",c);
				break;
			}
			case NODE_PREFIX: case NODE_PARALLEL: case NODE_HOLE: case NODE_CONTROL: {
				printf("ROOT\n");
				vector<node *> w = parser::bg_collapse(*it);
				node *n = new node();

				for(vector<node *>::iterator it2 = w.begin(); it2 != w.end(); ++it2) {
					n->add(*it2);
				}

				b->set_root(n);
				break;
			}
			case NODE_REACTION: {
				printf("NODE_REACTION\n");
				reactionnode *t = (reactionnode *)(*it);
				node *n1 = new node();
				vector<node *> w = bg_collapse(t->redex);

				for(vector<node *>::iterator it2 = w.begin(); it2 != w.end(); ++it2) {
					n1->add(*it2);
				}

				node *n2 = new node();
				w = bg_collapse(t->reactum);

				for(vector<node *>::iterator it2 = w.begin(); it2 != w.end(); ++it2) {
					n2->add(*it2);
				}

				b->add_rule(new reactionrule(n1,n2));
				break;
			}
			default:
				fprintf(stderr, "BUG: Fell through in parser::finish()\n");
				break;
		}
	}

	return b;
}

#include "bgparser.leg.cpp"
