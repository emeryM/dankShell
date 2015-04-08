/*-------------------------------------------------------------------
 * dank.h : contains all data structures and constants for dankShell
 *-------------------------------------------------------------------
 */

#include <string.h>
#include <unistd.h>

#define MAXCMDS 50
#define MAXARGS 100
#define MAXALIAS 100
#define MAXPATH 50
#define MAXPIPES 100

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
	int remote;
	int infd;
	int outfd;
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
	char *alname[MAXALIAS];
	char *alstr[MAXALIAS];
} ALIASTAB;

typedef struct pipetab{
	int * pipes[MAXPIPES];
}PIPETAB;

PIPETAB pipeHolder;
CMDTAB cmdtab;
ALIASTAB alias;
extern char **environ;
int builtin;
int alias_detected;
int currcmd; //current command
int cmdcount; //number of commands
int hasPipes;


