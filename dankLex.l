%{
#define YYSTYPE char *
#include <stdio.h>
#include "y.tab.h"
extern char* yylval;


%}

%%
[ \t]+     { ; }

bye        return EXIT;
ls         return LS;
cd         return CD;
setenv     return SETENV;
unsetenv   return UNSETENV;
printenv   return PRINTENV;
alias	   	 return ALIAS;
unalias    return UNALIAS;


"-"[a-zA-Z]+ 	yylval=strdup(yytext); return FLAG;

".."       return TWO_PERIODS;
";"        return SEMICOLON;
"("        return OPEN_PAREN;
")"        return CLOSE_PAREN;
"+"		   return PLUS;
"|"        return PIPE;
"\""       return QUOTE;
"<"        return OPEN_CARAT;
">"        return CLOSE_CARAT;
"\\"       return BACKSLASH;
"&"        return AMPERSAND;
"\n"       return EOLN;
"~/"       return HOME_PATH;
"~"		   return HOME;
"/"		   return ROOT;


[a-zA-Z/.]+  		yylval=strdup(yytext); return WORD;

\"([^\\\"]|\\.)*\"				yylval=strdup(yytext); return QUOTED;

[0-9]+     		yylval=strdup(yytext); return INTEGER;
[0-9.]+    		yylval=strdup(yytext); return DECIMAL;

%%

int yywrap(void){
		return 1;
}
