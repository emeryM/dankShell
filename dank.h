/*-------------------------------------------------------------------
 * dank.h : contains all data structures and constants for dankShell
 *-------------------------------------------------------------------
 */

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define MAXCMDS 50						// maximum commands on one line of input
#define MAXARGS 100						// maximum arguments for each command
#define MAXALIAS 100					// maximum aliases in the shell
#define MAXPIPES 100					// maximum pipes on one line of input
#define MAXINPUTLENGTH 2048		// maximum number of characters on a line of input

#define OK 0

#define KGRN "\x1B[32m"				// shell prompt color
#define KBLK "\x1B[0m"				// shell text color

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
	int  used;
	char *reparse_string;
	char *alname[MAXALIAS];
	char *alstr[MAXALIAS];
	int *found[MAXALIAS];
} ALIASTAB;

typedef struct pipetab{
	int * pipes[MAXPIPES];
}PIPETAB;

/* declare globals */

CMDTAB cmdtab;							// command table
int currcmd; 								// current command being processed
int cmdcount; 							// number of commands on a line of input
int builtin;					 			// nonzero if command is builtin

PIPETAB pipetab;						// pipetable
int has_pipes;							// indicates number of pipes on a line of input

ALIASTAB alias;							// alias table
int alias_detected;					// indicates presence of aliases on a line of input
int loop_detected;					// infinite alias loop detected

extern char **environ;			// allows access to shell environment variables
