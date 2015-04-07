%{
#define YYSTYPE char *
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dank.h"

%}

%token INTEGER DECIMAL
%token WORD FLAG EOLN QUOTED
%token LS CD EXIT PATH SETENV HOME_PATH HOME ROOT
%token PIPE QUOTE OPEN_CARAT CLOSE_CARAT BACKSLASH AMPERSAND PLUS SEMICOLON OPEN_PAREN CLOSE_PAREN TWO_PERIODS
%token UNSETENV PRINTENV ALIAS UNALIAS

%%
program:

			EXIT EOLN {printf("Smoke weed erry day...\n");
					exit(EXIT_SUCCESS);}
			| listFiles {return OK;}
			| changeDir {return OK;}
			| setEnvVar {return OK;}
			| setAlias  {return OK;}

			;

listFiles:
			LS EOLN{
				printf("no flag");
				builtin = 0;
				command.comname = "ls";
				command.nargs = 0;

            }

			|LS FLAG EOLN{
				printf(" found flag %s",$2 );
				builtin = 0;
				command.comname = "ls";
				command.atptr->args[1] = $2;
				command.nargs = 1;

			}

			;
changeDir:
			CD PATH EOLN{ printf(" change directory to %s ", $2);
					chdir($2);
				}
			|CD HOME_PATH WORD EOLN{ printf(" change directory to %s ", $3);
					chdir(getenv("HOME"));
					chdir($3);
				}
			|CD WORD EOLN{ printf(" change directory to %s ", $2);
					chdir($2);
				}

			|CD TWO_PERIODS PATH EOLN {printf(" change directory to %s ", $3);
					chdir("..");
					chdir($3);
				}

			|CD TWO_PERIODS WORD EOLN {printf(" change directory to %s ", $3);
					chdir("..");
					chdir($3);
				}

			|CD TWO_PERIODS EOLN{printf(" change directory up 1 " );
				chdir("..");
			}

			|CD EOLN {printf("change directory to home");
				chdir(getenv("HOME"));
			}
			|CD HOME EOLN {printf("change directory to home");
				chdir(getenv("HOME"));
			}
			|CD ROOT EOLN {printf("change directory to root");
				chdir("/");
			}
			|CD HOME PATH EOLN {printf("change directory to home and then some");
				chdir(getenv("HOME"));
				chdir($3);
			}
			;
setEnvVar:
			SETENV WORD PATH EOLN{
				command.comname = "setenv";
				command.nargs = 2;
				command.atptr->args[0] = $2;
				command.atptr->args[1] = $3;
				builtin = SETENV;
			}
			|UNSETENV WORD EOLN{
				command.comname = "unsetenv";
				command.nargs = 1;
				command.atptr->args[0] = $2;
				builtin = UNSETENV;
			}
			|PRINTENV EOLN{
				command.comname = "printenv";
				builtin = PRINTENV;
			}
			;
setAlias:
			ALIAS WORD QUOTED EOLN{
				command.comname = "alias";
				command.nargs = 2;
				command.atptr->args[0] = $2;
				char *q = $3;
				char *new =q+1;
				new[strlen(new)-1] = '\0';
				command.atptr->args[1] = new;
				builtin = ALIAS;
			}
			|ALIAS WORD WORD EOLN{
				command.comname = "alias";
				command.nargs = 2;
				command.atptr->args[0] = $2;
				command.atptr->args[1] = $3;
				builtin = ALIAS;
			}
			|ALIAS EOLN{
				command.comname = "alias";
				command.nargs = 0;
				builtin = ALIAS;
			}
			|UNALIAS WORD EOLN{
				command.comname = "unalias";
				command.nargs = 1;
				command.atptr->args[0] = $2;
				builtin = UNALIAS;
			}
			;



%%


int yyerror(char *s){
	fprintf(stderr, "*%s*\n", s);
}
