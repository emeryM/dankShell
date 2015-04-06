/*-------------------------------------------------------------------
 * dank.h : contains all data structures and constants for dankShell
 *-------------------------------------------------------------------
 */

#define MAXCMDS 50
#define MAXARGS 3000
#define MAXALIAS 100
#define MAXCACHE 100
#define MAXPATH 50

#define OK 0
#define SYSERR 1
#define ERRORS 2
#define OLD_ERR 3
#define SYSCALLER -1

#define BYE 4

#define THE_ONLY_ONE 1
#define FIRST 2
#define LAST 3

#define REGISTRY "REGISTRY"

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
	char *alname;
	char *alstr;
} ALIAS;

/* cache table structure */
typedef struct cache {
	char *cmd;
	char *hostport;
} CACHE;
