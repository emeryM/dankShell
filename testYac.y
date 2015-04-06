%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dank.h"
//#define YYSTYPE int;

%}

%token INTEGER DECIMAL 
%token WORD FLAG EOLN
%token LS CD EXIT PATH SETENV HOME_PATH HOME ROOT
%token PIPE QUOTE OPEN_CARAT CLOSE_CARAT BACKSLASH AMPERSAND PLUS SEMICOLON OPEN_PAREN CLOSE_PAREN TWO_PERIODS
%token UNSETENV PRINTENV

%% 
program: 	
		
			EXIT {printf("Smoke weed erry day...\n"); 
					exit(EXIT_SUCCESS);}
			| listFiles {fprintf(stderr, "dankShell: ");}
			| changeDir {fprintf(stderr, "dankShell: ");}
			| setEnvVar {fprintf(stderr, "dankShell: ");}
			
			;

listFiles: 	
			LS EOLN{  
				printf("no flag\n");
				forkAndExec("ls", "-a");
            }
			
			|LS FLAG EOLN{
				printf(" found flag %s\n",$2 );
				forkAndExec("ls", $2);
			}
			
			;
changeDir: 
			CD PATH EOLN{ printf(" change directory to %s \n", $2);
					chdir($2);
				}
			|CD HOME_PATH WORD EOLN{ printf(" change directory to %s \n", $3);
					chdir(getenv("HOME"));
					chdir($3);
				}
			|CD WORD EOLN{ printf(" change directory to %s \n", $2);
					chdir($2);
				}

			|CD TWO_PERIODS PATH EOLN {printf(" change directory to %s \n", $3);
					chdir("..");
					chdir($3);
				}

			|CD TWO_PERIODS WORD EOLN {printf(" change directory to %s \n", $3);
					chdir("..");
					chdir($3);
				}

			|CD TWO_PERIODS EOLN{printf(" change directory up 1 \n" );
				chdir("..");
			}
			
			|CD EOLN {printf("change directory to home\n");
				chdir(getenv("HOME"));
			}
			|CD HOME EOLN {printf("change directory to home\n");
				chdir(getenv("HOME"));
			}
			|CD ROOT EOLN {printf("change directory to root\n");
				chdir("/");
			}
			|CD HOME PATH EOLN {printf("change directory to home and then some\n");
				chdir(getenv("HOME"));
				chdir($3);
			}
			;
setEnvVar: 
			SETENV WORD PATH{
				printf("calling set env\n");
				command.comname = $2;
				command.nargs = 2;
				builtin = SETENV;

			}
			|UNSETENV WORD{
				//do stuff
			}
			|PRINTENV{
				//do stuff
			}
			;

			
%%

int main(void){
	fprintf(stderr, "dankShell: ");
	yyparse();
	return 0;
}

int yyerror(char *s){
	fprintf(stderr, "*%s*\n", s);
}