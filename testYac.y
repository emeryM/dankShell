%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define YYSTYPE int;

%}

%token INTEGER DECIMAL 
%token WORD FLAG EOLN
%token LS CD EXIT
%token PIPE QUOTE OPEN_CARAT CLOSE_CARAT BACKSLASH AMPERSAND PLUS SEMICOLON OPEN_PAREN CLOSE_PAREN TWO_PERIODS

%% 
program: 						{fprintf(stderr, "dankShell: ");}
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
			|LS EOLN{  
				printf("no flag\n");
				forkAndExec("ls", "-a");
             }
			program
			|LS FLAG {
	printf(" found flag %s\n",$2 );
				forkAndExec("ls", $2);
					}
					program
			;
changeDir: CD WORD { printf(" change directory to %s \n", $2);
					chdir($2);}
			program
			|CD TWO_PERIODS{printf(" change directory up 1 \n" );
				chdir("..");}
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


void forkAndExec(char * input, char * flag){

				printf("in forkanexec\n");
			   int process = fork ();
			   char * bin = "/bin/";

	           	char buf[512];
			   snprintf(buf, sizeof buf, "%s%s", bin, input);

	           if (process > 0){             
	              wait ((int *) 0);      
	           }else if (process == 0){ 
	           	
	           	
	              execl( buf, input, flag, (char*)0 );
	                                   
	              fprintf (stderr, "Can't execute \n");
	              exit (1);
	              
	           }else if(process == -1){

	              fprintf (stderr, "Can't fork!\n");
	              exit (2);
	              }
	}