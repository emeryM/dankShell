/*-------------------------------------------------------------------
 * main.c : the main loop of dankShell
 *-------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>

#include "dank.h"
#include "y.tab.h"

void shell_init(){
	printf("%s ", "INITIALIZING SHELL...");
	//char *path = getenv("PATH");
	//printf("\nPATH = %s", path);
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
	if( yyparse() ){
		printf("\n%s ", "getting here! err");
		understand_errors();
	}
	else{
		printf("\n%s ", "getting here!");
		return 1;
	}
	printf("\n%s ", "getting here blah!");
	return 1;
}

void execute_builtin(){
	switch( builtin ){
		case SETENV:
		printf("\nCommand is: %s", command.comname);
		exit( EXIT_SUCCESS );
		break;
		case PRINTENV:
		break;
		case UNSETENV:
		break;
		case CD:
		break;
		/*case ALIAS:
		break;
		case UNALIAS:
		break;*/
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
			case 1:
				printf("\n%s ", "OK!");
				process_command();
				break;
			case 2:
				printf("\n%s ", "Error!");
				recover_from_errors();
				break;
			case 3:
				printf("\n%s\n", "Goodbye!");
				exit( EXIT_SUCCESS );
				break;
			default:
				printf("%c ", CMD);
				break;
		}
	}
}
