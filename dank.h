/*-------------------------------------------------------------------
 * dank.h : contains all data structures and constants for dankShell
 *-------------------------------------------------------------------
 */

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define MAXCMDS 50
#define MAXARGS 100
#define MAXALIAS 100
#define MAXPATH 50
#define MAXPIPES 100
#define MAXINPUTLENGTH 2048

#define OK 0
#define SYSERR 1
#define ERRORS 2
#define OLD_ERR 3
#define SYSCALLER -1

#define THE_ONLY_ONE 1
#define FIRST 2
#define LAST 3

#define KGRN "\x1B[32m"
#define KBLK "\x1B[0m"

/* command arguments structures */
typedef struct cmdargs {
	char *args[MAXARGS];
} ARGTAB;

/* command structure */
typedef struct cmd {
	char	*cmdname;
	int infd;
	int outfd;
	int errfd;
	int nargs;
	ARGTAB *atptr;
}	COMMAND;

/* command table */
typedef struct cmdtab {
	COMMAND cmd[MAXCMDS];
} CMDTAB;

/* alias structure */
typedef struct alias {
	int used;
	char *reparse_string;
	char *alname[MAXALIAS];
	char *alstr[MAXALIAS];
} ALIASTAB;

typedef struct pipetab{
	int * pipes[MAXPIPES];
}PIPETAB;

/* declare globals */

CMDTAB cmdtab;
int currcmd; //current command
int cmdcount; //number of commands
int builtin;

PIPETAB pipeHolder;
int hasPipes;

ALIASTAB alias;
int alias_detected;

extern char **environ;
