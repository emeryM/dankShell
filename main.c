/*-------------------------------------------------------------------
 * main.c : the main loop of dankShell
 *-------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>

#include "dank.h"
#include "y.tab.h"
#define KGRN "\x1B[32m"
#define KBLK "\x1B[0m"

void shell_init(){
	printf("%s ", "INITIALIZING SHELL...");
	command.atptr = &argtab;
}

void print_prompt(){
	printf(KGRN "\ndankShell:");
	printf(KBLK " ");
}

void init_scanner_and_parser(){

}

void understand_errors(){
	printf("There were errors");
}

int get_command(){
	init_scanner_and_parser();
	if( yyparse() ){
		understand_errors();
		return 1;
	}
	else{
		if( builtin == EXIT ){
			return 2;
		}
		return 0;
	}
}

void execute_builtin(){
	switch( builtin ){
		case SETENV:
		setenv(command.atptr->args[0], command.atptr->args[1], 1);
		char* test = getenv(command.atptr->args[0]);
		printf("\n%s", test);
		break;
		case PRINTENV:
		break;
		case UNSETENV:
		break;
		case CD:
		break;
		case ALIAS:
		break;
		case UNALIAS:
		break;
		case EXIT:
		break;
	}
}

void execute_command(){

}

void process_command(){
	if ( builtin ){
		execute_builtin();
	}
	else{
		execute_command();
	}
}

void recover_from_errors(){

}

int main() {
	shell_init();
	while(1){
		print_prompt();
		int CMD = get_command();
		switch( CMD ){
			case 0:
				process_command();
				break;
			case 1:
				recover_from_errors();
				break;
			case 2:
				printf("\n%s\n", "Goodbye!");
				exit( EXIT_SUCCESS );
				break;
			default:
				printf("%c ", CMD);
				break;
		}
	}
}
