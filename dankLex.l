%{
#define YYSTYPE char *
#include <stdio.h>
#include "y.tab.h"
extern char* yylval;

         #define YY_INPUT(buf,result,max_size) \
             { \
             int c = getchar(); \
             result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
             }



%}

%%
[ \t]+     { ; }

bye        return EXIT;

cd         return CD;
setenv     return SETENV;
unsetenv   return UNSETENV;
printenv   return PRINTENV;
alias	   	 return ALIAS;
unalias    return UNALIAS;


<<EOF>>     return END_OF_FILE;

"*"		   return WILDCARDWORD;
"?"		   return WILDCARDCHAR;
".."       return TWO_PERIODS;
";"        return SEMICOLON;
"("        return OPEN_PAREN;
")"        return CLOSE_PAREN;
"+"		   	 return PLUS;
"|"        return PIPE;
"\""       return QUOTE;
"<"        return OPEN_CARET;
">"        return CLOSE_CARET;
">>"   	   return DBL_CLOSE_CARET;
"2>"	  	 return ERROR_CARET;
"\\"       return BACKSLASH;
"&"        return AMPERSAND;
"\n"       return EOLN;
"~/"       return HOME_PATH;
"~"		  	 return HOME;
"/"		  	 return ROOT;
"\$\{"		 return OPEN_VAR;
"\}"			 return CLOSE_VAR;

"-"?[a-zA-Z0-9/_\-.:*?]+ 		yylval=strdup(yytext); return WORD;

\"([^\\\"]|\\.)*\"				yylval=strdup(yytext); return QUOTED;



[0-9]+     		yylval=strdup(yytext); return INTEGER;
[0-9.]+    		yylval=strdup(yytext); return DECIMAL;

%%



int yywrap(void){
		return 1;
}
