%{
#include <stdio.h>
#include <y.tab.h>
#define YYSTYPE int;

%}

%token INTEGER PLUS WORD 

%% 
list: /*nothing*/
		|list addthings
		;
addthings: INTEGER PLUS INTEGER { printf("%f\n",($1+$3)); }
%%

int main(void){
	yyparse();
	return 0;
}

int yyerror(char *s){
	fprintf(stderr, "*%s*\n", s)
}