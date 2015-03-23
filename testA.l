%{
#include <stdio.h>
#include "y.tab.h"
extern int yylval;


%}

%% 
[a-z]     return LWORD;
[A-Z]     return UWORD;
[a-zA-Z]  return WORD;
[0-9]     yylval=atoi(yytext); return INTEGER;
[0-9.]    yylval=atoi(yytext); return DECIMAL;
";"       return SEMICOLON;
"("       return OPEN_PAREN;
")"       return CLOSE_PAREN;
"+"		  return PLUS;
%%

