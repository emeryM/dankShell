/*-------------------------------------------------------------------
 * dank.h : contains all data structures and constants for dankShell
 *-------------------------------------------------------------------
 */

#include <string.h>
#include <unistd.h>

#define MAXCMDS 50
#define MAXARGS 300
#define MAXALIAS 100
#define MAXCACHE 100
#define MAXPATH 50

#define OK 0
#define SYSERR 1
#define ERRORS 2
#define OLD_ERR 3
#define SYSCALLER -1

#define THE_ONLY_ONE 1
#define FIRST 2
#define LAST 3

#define REGISTRY "REGISTRY"

#define KGRN "\x1B[32m"
#define KBLK "\x1B[0m"

/* command line arguments structures */
typedef struct comargs {
	char *args[MAXARGS];
} ARGTAB;

/* command line structure */
typedef struct com {
	char	*comname;
	int remote;
	int infd;
	int outfd;
	int nargs;
	ARGTAB *atptr;
}	COMMAND;

/* alias structure */
typedef struct alias {
	int used;
	char *alname[MAXALIAS];
	char *alstr[MAXALIAS];
} ALIASTAB;

/* cache table structure */
typedef struct cache {
	char *cmd;
	char *hostport;
} CACHE;

COMMAND command;
ARGTAB argtab;
ALIASTAB alias;
CACHE cache;
extern char **environ;
int builtin;
