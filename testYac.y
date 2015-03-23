%{
#include <stdio.h>
#include <stdlib.h>
//#define YYSTYPE int;

%}

%token INTEGER PLUS //WORD UWORD LWORD DECIMAL SEMICOLON OPEN_PAREN CLOSE_PAREN

%% 

addthings: INTEGER PLUS INTEGER { printf("%f\n",($1+$3)); }
%%

int main(void){
	yyparse();
	return 0;
}

int yyerror(char *s){
	fprintf(stderr, "*%s*\n", s);
}