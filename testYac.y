%{
#include <stdio.h>
#include <stdlib.h>
//#define YYSTYPE int;

%}

%token INTEGER DECIMAL 
%token WORD
%token LS CD EXIT
%token PIPE QUOTE OPEN_CARAT CLOSE_CARAT BACKSLASH AMPERSAND PLUS SEMICOLON OPEN_PAREN CLOSE_PAREN TWO_PERIODS

%% 
program:
		addthings '\n'
		| listFiles
		| changeDir
		| EXIT {printf("Smoke weed erry day...\n"); 
				exit(EXIT_SUCCESS);}
		;

addthings: INTEGER PLUS INTEGER { printf("the result is %d\n",($1+$3)); }
			program
		;

listFiles: LS
			{execl( "/bin/ls", "ls", "-l", (char*)0 );}
			program
			;
changeDir: CD WORD { printf(" change directory \n");}
			program
			|CD TWO_PERIODS{printf(" change directory up 1 \n" );}
			program
			;
%%

int main(void){
	yyparse();
	return 0;
}

int yyerror(char *s){
	fprintf(stderr, "*%s*\n", s);
}