%{
#include <stdio.h>
#include <y.tab.h>
extern YYSTYPE yylval;

%}

%% 
[a-zA-Z]  return WORD;
[A-Z]     return UPPERWORD;
[a-z]     return LOWERWORD;
[0-9]     yylval=atoi(yytext); return INTEGER;
[0-9.]    yylval=atoi(yytext); return DECIMAL;
";"       return SEMICOLON;
"("       return OPEN_PAREN;
")"       return CLOSE_PAREN;
"+"		  return PLUS;
%%