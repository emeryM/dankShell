%{
#include <stdio.h>
#include "y.tab.h"



%}

%%
[ \t]+     { ; }

bye        return EXIT;
ls         return LS;
cd         return CD;
setenv     return SETENV;
unsetenv   return UNSETENV;
printenv   return PRINTENV;


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


[a-zA-Z]+  		yylval=strdup(yytext); return WORD;
[a-zA-Z/.]+		yylval=strdup(yytext); return PATH;

[0-9]+     		yylval=strdup(yytext); return INTEGER;
[0-9.]+    		yylval=strdup(yytext); return DECIMAL;


%%

int yywrap(void){
		return 1;
}
