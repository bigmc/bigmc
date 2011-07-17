#ifndef _PARSER_H
#define _PARSER_H

class parser {
	FILE *fp;
	static node		*bg_mknode(prefixnode *p);
	static node		*bg_mknode(parallelnode *p);
	static node		*bg_mknode(holenode *p);
	static node		*bg_mknode(parsenode *p);
	static node		*bg_mknode(controlnode *p);
	static control		bg_mkctrl(controlnode *p);
	static string		bg_mkname(namenode *p);
	static vector<node *>	bg_collapse(parallelnode *p);
	static vector<node *>	bg_collapse(parsenode *p);

public:
	static void init(char *file);
	static void cleanup();
	static int  parse();
	static void add_result(parsenode *p);
	static bigraph *finish();
};

extern vector<parsenode *> g_declaration;
extern parsenode **yylval;
int parser_next_char();
void parser_add_result(parsenode *p);
int yyparse();

#endif
