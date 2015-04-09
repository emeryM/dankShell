%{
#define YYSTYPE char *
#include <stdio.h>
#include <stdlib.h>
#include "dank.h"
%}

%token INTEGER DECIMAL
%token WORD FLAG EOLN QUOTED
%token CD EXIT SETENV HOME_PATH HOME ROOT
%token PIPE QUOTE OPEN_CARET CLOSE_CARET DBL_CLOSE_CARET ERROR_CARET BACKSLASH AMPERSAND PLUS SEMICOLON OPEN_PAREN CLOSE_PAREN TWO_PERIODS
%token UNSETENV PRINTENV ALIAS UNALIAS

%%
program:

			  goodbye 	{return OK;}
			| changeDir {return OK;}
			| setEnvVar {return OK;}
			| setAlias  {return OK;}
			| piping    {return OK;}
			| commands  {
				if( alias_detected > 0 ){
					alias_detected = 0;
					int i;
					int j;
					for( j = 0; j <= currcmd; ++j){
						for( i = 0; i <= cmdtab.cmd[j].nargs; ++i ){
							strncat(
								alias.reparse_string,
								cmdtab.cmd[j].atptr->args[i],
								MAXINPUTLENGTH-strlen(alias.reparse_string)
							);
							strncat(
								alias.reparse_string,
								" ",
								MAXINPUTLENGTH-strlen(alias.reparse_string)
							);
						}
						if( hasPipes > 0 && j != currcmd ){
							strncat(
								alias.reparse_string,
								"| ",
								MAXINPUTLENGTH-strlen(alias.reparse_string)
							);
						}
					}
					printf("\nThe expanded command is: %s\n", alias.reparse_string);
					yy_scan_string(alias.reparse_string);

					return 570;
				}
				else{
					yyrestart(stdin);
					return OK;
				}
				}

			;
goodbye:
			EXIT EOLN{
				builtin = EXIT;
			}
			;

changeDir:
			CD WORD EOLN{ printf(" change directory to %s ", $2);
					chdir($2);
					builtin = CD;
				}
			|CD HOME_PATH WORD EOLN{ printf(" change directory to %s ", $3);
					chdir(getenv("HOME"));
					chdir($3);
					builtin = CD;
				}

			|CD TWO_PERIODS WORD EOLN {printf(" change directory to %s ", $3);
					chdir("..");
					chdir($3);
					builtin= CD;
				}

			|CD TWO_PERIODS EOLN{printf(" change directory up 1 " );
				chdir("..");
				builtin= CD;
			}

			|CD EOLN {printf("change directory to home");
				chdir(getenv("HOME"));
				builtin= CD;
			}
			|CD HOME EOLN {printf("change directory to home");
				chdir(getenv("HOME"));
				builtin= CD;
			}
			|CD ROOT EOLN {printf("change directory to root");
				chdir("/");
				builtin= CD;
			}
			|CD HOME WORD EOLN {printf("change directory to home and then some");
				chdir(getenv("HOME"));
				chdir($3);
				builtin= CD;
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
commands:
			WORD{


				int i = 0;
				while( i < alias.used && strcmp($1, alias.alname[i])){
					++i;
				}
				if( i < alias.used ){
					alias_detected++;
					strcpy($1,alias.alstr[i]);
				}

				printf("first thing cmmand name is: %s\n", $1);

				cmdtab.cmd[currcmd].cmdname = $1;
				cmdtab.cmd[currcmd].nargs = 0;
				cmdtab.cmd[currcmd].atptr->args[0] = $1;
				builtin = 0;
			}
			|commands CLOSE_CARET WORD{
				printf("Yacc detected file output redirect");
				cmdtab.cmd[currcmd].outfd = open($3, O_WRONLY | O_CREAT ,0755);

			}
			|commands DBL_CLOSE_CARET WORD{
				printf("Yacc detected file output redirect");
				cmdtab.cmd[currcmd].outfd = open($3, O_WRONLY | O_APPEND | O_CREAT,0755);
				
			}
			|commands ERROR_CARET WORD{
				printf("Yacc detected file error redirect");
				cmdtab.cmd[currcmd].errfd = open($3, O_WRONLY | O_APPEND | O_CREAT,0755);
				
			}
			|commands WORD{

				cmdtab.cmd[currcmd].nargs++;
				printf("we dem args, nargs: %d\n", cmdtab.cmd[currcmd].nargs);
				printf("the arg is: %s\n",$2 );
				cmdtab.cmd[currcmd].atptr->args[cmdtab.cmd[currcmd].nargs] = $2;
			}
			|commands PIPE WORD{
				printf("after pipe command name is : %s\n", $3);

				int i = 0;
				while( i < alias.used && strcmp($3, alias.alname[i])){
					printf("\nLooping");
					++i;
				}
				if( i >= alias.used ){
					printf("\nNo alias found");
				}
				else{
					printf("\nAlias found");
					alias_detected++;
					strcpy($3,alias.alstr[i]);
				}

				printf("Command is %s \n", $3);

				++hasPipes;
				++currcmd;
				++cmdcount;
				cmdtab.cmd[currcmd].cmdname = $3;
				cmdtab.cmd[currcmd].nargs = 0;
				cmdtab.cmd[currcmd].atptr->args[0] = $3;
				builtin = 0;

			}
			;



%%


int yyerror(char *s){
	fprintf(stderr, "*%s*\n", s);
	return 1;
}
