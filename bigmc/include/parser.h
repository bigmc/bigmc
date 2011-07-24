#ifndef _PARSER_H
#define _PARSER_H

class parser {
	FILE *fp;
	static term		*bg_mknode(prefixnode *p);
	static term		*bg_mknode(parallelnode *p);
	static term		*bg_mknode(holenode *p);
	static term		*bg_mknode(parsenode *p);
	static term		*bg_mknode(controlnode *p);
	static control		bg_mkctrl(controlnode *p);
	static string		bg_mkname(namenode *p);
	static vector<name>	bg_names(seqnode *p);
	static vector<name>	bg_names(parsenode *p);
	static set<term *> 	bg_collapse(parallelnode *p); 
	static set<term *> 	bg_collapse(parsenode *p);
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
int yylex();
int yyparse();

#endif
