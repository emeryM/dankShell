%{
#define YYSTYPE char *
#include <stdio.h>
#include <stdlib.h>
#include "dank.h"

%}

%token INTEGER DECIMAL
%token WORD FLAG EOLN QUOTED
%token LS CD EXIT SETENV HOME_PATH HOME ROOT
%token PIPE QUOTE OPEN_CARAT CLOSE_CARAT BACKSLASH AMPERSAND PLUS SEMICOLON OPEN_PAREN CLOSE_PAREN TWO_PERIODS
%token UNSETENV PRINTENV ALIAS UNALIAS

%%
program:

			  goodbye 	{return OK;}
			| listFiles {return OK;}
			| changeDir {return OK;}
			| setEnvVar {return OK;}
			| setAlias  {return OK;}
			| piping    {return OK;}
			| commands  {return OK;}

			;
goodbye:
			EXIT EOLN{
				builtin = EXIT;
			}
			;
listFiles:
			LS EOLN{
				printf("no flag");
				builtin = 0;
				cmdtab.cmd[currcmd].cmdname = "ls";
				cmdtab.cmd[currcmd].nargs = 0;

            }

			|LS FLAG EOLN{
				printf(" found flag %s",$2 );
				builtin = 0;
				cmdtab.cmd[currcmd].cmdname = "ls";
				cmdtab.cmd[currcmd].atptr->args[1] = $2;
				cmdtab.cmd[currcmd].nargs = 1;

			}

			;
changeDir:
			CD WORD EOLN{ printf(" change directory to %s ", $2);
					chdir($2);
				}
			|CD HOME_PATH WORD EOLN{ printf(" change directory to %s ", $3);
					chdir(getenv("HOME"));
					chdir($3);
				}
			|CD WORD EOLN{ printf(" change directory to %s ", $2);
					chdir($2);
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
			|CD HOME WORD EOLN {printf("change directory to home and then some");
				chdir(getenv("HOME"));
				chdir($3);
			}
			;
setEnvVar:
			SETENV WORD WORD EOLN{
				cmdtab.cmd[currcmd].cmdname = "setenv";
				cmdtab.cmd[currcmd].nargs = 2;
				cmdtab.cmd[currcmd].atptr->args[0] = $2;
				cmdtab.cmd[currcmd].atptr->args[1] = $3;
				builtin = SETENV;
			}
			|UNSETENV WORD EOLN{
				cmdtab.cmd[currcmd].cmdname = "unsetenv";
				cmdtab.cmd[currcmd].nargs = 1;
				cmdtab.cmd[currcmd].atptr->args[0] = $2;
				builtin = UNSETENV;
			}
			|PRINTENV EOLN{
				cmdtab.cmd[currcmd].cmdname = "printenv";
				builtin = PRINTENV;
			}
			;
setAlias:
			ALIAS WORD QUOTED EOLN{
				cmdtab.cmd[currcmd].cmdname = "alias";
				cmdtab.cmd[currcmd].nargs = 2;
				cmdtab.cmd[currcmd].atptr->args[0] = $2;
				char *q = $3;
				char *new =q+1;
				new[strlen(new)-1] = '\0';
				cmdtab.cmd[currcmd].atptr->args[1] = new;
				builtin = ALIAS;
			}
			|ALIAS WORD WORD EOLN{
				cmdtab.cmd[currcmd].cmdname = "alias";
				cmdtab.cmd[currcmd].nargs = 2;
				cmdtab.cmd[currcmd].atptr->args[0] = $2;
				cmdtab.cmd[currcmd].atptr->args[1] = $3;
				builtin = ALIAS;
			}
			|ALIAS EOLN{
				cmdtab.cmd[currcmd].cmdname = "alias";
				cmdtab.cmd[currcmd].nargs = 0;
				builtin = ALIAS;
			}
			|UNALIAS WORD EOLN{
				cmdtab.cmd[currcmd].cmdname = "unalias";
				cmdtab.cmd[currcmd].nargs = 1;
				cmdtab.cmd[currcmd].atptr->args[0] = $2;
				builtin = UNALIAS;
			}
			;
piping:
			PIPE EOLN{
				printf("piping shit \n");
				cmdtab.cmd[currcmd].cmdname = "pipe";
				builtin = 0;
			}
			;



%%


int yyerror(char *s){
	fprintf(stderr, "*%s*\n", s);
}
