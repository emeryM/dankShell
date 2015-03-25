%{
#include <stdio.h>
#include "y.tab.h"
extern int yylval;


%}

%% 
exit       return EXIT;
ls         return LS;
cd         return CD;

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

[a-zA-Z]+  return WORD;
[0-9]+     yylval=atoi(yytext); return INTEGER;
[0-9.]+    yylval=atoi(yytext); return DECIMAL;


%%

int yywrap(void){
		return 1;
}

