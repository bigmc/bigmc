%{
#include<sstream>
#include "bigmc.h"
#include "bgparser.h"

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

%x COMMENT

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
"||"       	  {
                    return LOR;
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
"%property"       {
                    return PROPERTY;
                  }
"&&"       	  {
                    return LAND;
                  }
"!"		  {
                    return LNOT;
		  }
"%check"          {
                    yyterminate();
                  }
":"               {
                    return COLON;
                  }
"<="		  { return LEQ; }
">="		  { return GEQ; }
"<"		  { return LT; }
">"		  { return GT; }
"=="		  { return EQ; }
"!="		  { return NEQ; }
"forall"	  { return FORALL; }
"exists"	  { return EXISTS; }
"$"		  { return DOLLAR; }
"true"		  { return BTRUE; }
"if"		  { return IF; }
"then"		  { return THEN; }
"else"		  { return ELSE; }
"#"		  { BEGIN COMMENT; }

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
<COMMENT>"\n"	  { yycolumn = 1; BEGIN 0; }
<COMMENT>.	  { ; }

%%
