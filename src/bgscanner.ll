%{
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
#include<sstream>
#include "bigmc.h"
#include "bgparser.hpp"

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
IDENT1	[a-zA-Z+]
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
"%name"           {
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
"%rule"		  {
		    return RULE;
		  }
"%import"         {
		    return IMPORT;
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
"-"		  { return UNLINKED; }
"nil"		  { return NIL; }

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
