/*-------------------------------------------------------------------
 * dank.h : contains all data structures and constants for dankShell
 *-------------------------------------------------------------------
 */

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

COMMAND command;
ARGTAB argtab;
ALIAS alias;
CACHE cache;

int builtin;
/*
	builtin values
	0	FALSE - not a builtin
	1	setenv variable word
	2	printenv
	3	unsetenv variable
	4	cd word_directory_name
	5	alias
	6	alias name word
	7	unalias name
	8	bye
*/

#define _SETENV 1
#define _PRINTENV 2
#define _UNSETENV 3
#define _CD 4
#define _ALIAS 5
#define _UNALIAS 6
#define _BYE 7
