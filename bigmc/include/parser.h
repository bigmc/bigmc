#ifndef _PARSER_H
#define _PARSER_H

class parser {
	FILE *fp;

public:
	parser(char *file);
	~parser();
	bigraph parse();
};

extern vector<parsenode *> g_declaration;
extern parsenode **yylval;
int parser_next_char();

#endif
