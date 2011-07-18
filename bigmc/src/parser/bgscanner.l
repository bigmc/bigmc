%{
#include<sstream>
#include "bigmc.h"
#include "bgparser.tab.hpp"

int line = 0;
int column = 0;

#define YY_INPUT(buf, result, max_size)        \
{                                              \
	int yyc = parser_next_char();                    \
	result = (EOF == yyc) ? 0 : (*(buf)= yyc, 1); \
}

int yycolumn = 1;

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng;

%}

%option noyywrap
%option yylineno

DIGIT	[0-9]
IDENT1	[a-zA-Z]
IDENTR	[a-zA-Z0-9_]

%%

" "               {
                  }
"\t"              {
                  }
"\n"		  {
			yycolumn = 1;
		  }
";"               {
                    return SEMI;
                  }
"->"               {
                    return ARROW;
                  }
"|"               {
                    return PAR;
                  }
"."               {
                    return PREF;
                  }
","               {
                    return COMMA;
                  }
"["               {
                    return LSQ;
                  }
"]"               {
                    return RSQ;
                  }
"("               {
                    return LPAR;
                  }
")"               {
                    return RPAR;
                  }
"%inner"          {
                    return INNER;
                  }
"%outer"          {
                    return OUTER;
                  }
"%active"         {
                    return ACTIVE;
                  }
"%passive"        {
                    return PASSIVE;
                  }
"%check"          {
                    yyterminate();
                  }
":"               {
                    return COLON;
                  }
"$"{DIGIT}+ 	  {
		    yylval.ident = strdup(yytext);
                    return HOLE;
                  }
{DIGIT}+	 {
		    std::istringstream(yytext) >> yylval.value;
                    return NUM;
                  }
{IDENT1}{IDENTR}*  {
		    yylval.ident = strdup(yytext);
                    return IDENT;
                  }
.                 {
                    return UNKNOWN;
                  }
<<EOF>>           {
                    yyterminate();
                  }

%%
