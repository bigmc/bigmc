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

#define YY_INPUT(buf, result, max_size)        \
{                                              \
	int yyc = parser_next_char();                    \
	result = (EOF == yyc) ? 0 : (*(buf)= yyc, 1); \
}

#include "bgparser.leg.cpp"

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
			return EOF;
		}

		g_inputbuffer_len = strlen(g_inputbuffer);
		g_inputbuffer_offset = 0;
	}

	if(g_inputbuffer_len == 0) return '\n';
	if(*g_inputbuffer == '\0' || *g_inputbuffer == EOF) {
		exit(0);
	}

	if(strcmp("exit",g_inputbuffer) == 0 || strcmp("quit",g_inputbuffer) == 0) {
		exit(0);
	}

	return g_inputbuffer[g_inputbuffer_offset++];
}

parser::parser(char *file) {
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

parser::~parser() {
	if(g_fp)
		fclose(g_fp);
}

bigraph parser::parse() {
	while(!g_userbreak && yyparse()) {
	}

	//string s = (*yyval)->to_string();

	//cout << "S: " << s << "\n";

	bigraph b(1);

	return b;
}
