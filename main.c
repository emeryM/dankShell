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

	//printf("in forkanexec\n");
    int process = fork ();
    /*
    char * bin = "/bin/";

   	char buf[512];
    snprintf(buf, sizeof buf, "%s%s", bin, input);
	*/

    if (process > 0){             
		wait ((int *) 0);      
    }else if (process == 0){ 
   	
   		command.atptr->args[0]= command.comname;
    	execvp( command.comname,command.atptr->args );
                           
    	fprintf (stderr, "Can't execute \n");
    	exit (1);
      
    }else if(process == -1){

    	fprintf (stderr, "Can't fork!\n");
    	exit (2);
    }

}

void process_command(){
	if ( builtin ){
		execute_builtin();
	}
	else{
		//this if statement is for testing pipes
		printf("calling : %s\n",command.comname );
		if(strcmp(command.comname,"pipe") == 0){
			printf("calling piped and sniped: %s\n",command.comname );
			piped_and_sniped();}
		else{
		execute_command();
		clear_args();
		}
	}
}
void piped_and_sniped(){
	  int des_p[2];
        if(pipe(des_p) == -1) {
          perror("Pipe failed");
          exit(1);
        }

        if(fork() == 0)        //first fork
        {
            close(1);          //closing stdout
            dup(des_p[1]);     //replacing stdout with pipe write 
            close(des_p[0]);   //closing pipe read
            close(des_p[1]);

            const char* prog1[] = { "ls", "-l", 0};
            execvp(prog1[0], prog1);
            perror("execvp of ls failed");
            exit(1);
        }

        if(fork() == 0)        //creating 2nd child
        {
            close(0);          //closing stdin
            dup(des_p[0]);     //replacing stdin with pipe read
            close(des_p[1]);   //closing pipe write
            close(des_p[0]);

            const char* prog2[] = { "wc", "-l", 0};
            execvp(prog2[0], prog2);
            perror("execvp of wc failed");
            exit(1);
        }

        close(des_p[0]);
        close(des_p[1]);
        wait(0);
        wait(0);

	
}

void clear_args(){
	//printf("clearing %d args\n" ,command.nargs);
	int i;
	for (i = 0; i < command.nargs+1; ++i)
	{
		command.atptr->args[i] = NULL;
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
