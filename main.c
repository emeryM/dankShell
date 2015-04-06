/*-------------------------------------------------------------------
 * main.c : the main loop of dankShell
 *-------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>

#include "dank.h"

COMMAND command;
ARGTAB argtab;
ALIAS alias;
CACHE cache;

void shell_init(){
	printf("%s ", "INITIALIZING SHELL...");
	char *path = getenv("PATH");
	printf("\nPATH = %s", path);
}

void print_prompt(){
	printf("\n%s", "dankShell: ");
}

void init_scanner_and_parser(){
	
}

void understand_errors(){
	
}

int get_command(){
	init_scanner_and_parser();
	/*
	if(yyparse()){
		understand_errors();
	}
	else{
		return OK;
	}
	*/
	//TEMPORARY
	return BYE;
}

void process_command(){
	//TESTING
	command.comname = "";
}

void recover_from_errors(){
	
}

int main() {
	shell_init();
	while(1){
		print_prompt();
		int CMD = get_command();
		switch( CMD ){
			case OK:
				printf("%s ", "OK!");
				process_command();
				break;
			case ERRORS:
				printf("%s ", "Error!");
				recover_from_errors();
				break;
			case BYE:
				printf("\n%s\n", "Goodbye!");
				exit( EXIT_SUCCESS );
				break;
			default:
				printf("%c ", CMD);
				break;
		}
	}
}
