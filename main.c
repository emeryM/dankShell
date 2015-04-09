/*-------------------------------------------------------------------
 * main.c : the main loop of dankShell
 *-------------------------------------------------------------------
 */

#include "dank.h"
#include "y.tab.h"

/* shell_init: initialize globals */
void shell_init(){
	int i;
	//initialize command table and variables
	for (i= 0; i < MAXCMDS; ++i){
		ARGTAB *args = malloc(sizeof(ARGTAB));
		COMMAND cmd;
		cmd.cmdname = NULL;
		cmd.infd = -1;
		cmd.outfd = -1;
		cmd.nargs = 0;
		cmd.atptr = args;
		cmdtab.cmd[i] = cmd;
	}
	currcmd = 0;
	cmdcount = 0;
	builtin = 0;
	//initialize pipe table and variables
	for(i = 0; i < MAXPIPES; ++i){
		int* arr = (int*)malloc(2 * sizeof(int));
		pipeHolder.pipes[i] = arr;
	}
	hasPipes = 0;
	//initialize alias table and variables
	alias.used = 0;
	char alstring[MAXINPUTLENGTH] = {0};
	alias.reparse_string = alstring;
	alias_detected = 0;
	//initialize other variables
}

/* print_prompt: print prompt to shell */
void print_prompt(){
	printf(KGRN "\ndankShell:");
	printf(KBLK " ");
}


void clear_args(){
	int i;
	int j;
	//clear command table and variables
	for(currcmd = 0; currcmd <= cmdcount; ++currcmd){
		cmdtab.cmd[currcmd].cmdname = NULL;
		cmdtab.cmd[currcmd].infd = -1;
		cmdtab.cmd[currcmd].outfd = -1;
		for (i = 0; i < cmdtab.cmd[currcmd].nargs+1; ++i)
		{
			cmdtab.cmd[currcmd].atptr->args[i] = NULL;
		}
		cmdtab.cmd[currcmd].nargs = 0;
	}
	currcmd = 0;
	cmdcount = 0;
	builtin = 0;
	//clear pipe variables
	hasPipes = 0;
	//clear alias variables
	alias_detected = 0;
	memset(alias.reparse_string,0,strlen(alias.reparse_string));
	builtin = 0;
}

int get_command(){
	if( yyparse() ){
		return 1;
	}
	else{
		if( builtin == EXIT ){
			return 2;
		}
		return 0;
	}
}

void list_aliases(){
	if( alias.used == 0 ){
		printf("No aliases to display.");
	}
	else{
		int i;
		for(i = 0; i < alias.used; ++i ){
			printf("\n%s\t%s", alias.alname[i], alias.alstr[i]);
		}
	}
}

void set_alias(){
	if (alias.used >= MAXALIAS){
		printf("\nToo many aliases already exist.");
	}
	else{
		int i = 0;
		while( i < alias.used && strcmp(cmdtab.cmd[currcmd].atptr->args[0], alias.alname[i]) ){
			++i;
		}
		alias.alname[i] = cmdtab.cmd[currcmd].atptr->args[0];
		alias.alstr[i] = cmdtab.cmd[currcmd].atptr->args[1];
		if( i == alias.used ){
			++alias.used;
		}
	}
}

void remove_alias(){
	int i = 0;
	while( i < alias.used && strcmp(cmdtab.cmd[currcmd].atptr->args[0], alias.alname[i]) ){
		++i;
	}
	if( i >= alias.used ){
		printf("Alias not found");
	}
	else{
		while( i < alias.used ){
			alias.alname[i] = alias.alname[i+1];
			alias.alstr[i] = alias.alstr[i+1];
			++i;
		}
		alias.alname[alias.used] = "";
		alias.alstr[alias.used--] = "";
	}
}

void execute_builtin(){
	switch( builtin ){
		case SETENV:
			setenv(cmdtab.cmd[currcmd].atptr->args[0], cmdtab.cmd[currcmd].atptr->args[1], 1);
			break;
		case PRINTENV: ;
		char **current = environ;
			while(*current){
				puts(*current);
				 current++;
			}
			break;
		case UNSETENV:
			unsetenv(cmdtab.cmd[currcmd].atptr->args[0]);
			break;
		case CD:
			break;
		case ALIAS:
			if(cmdtab.cmd[currcmd].nargs == 0){
				list_aliases();
			}
			else{
				set_alias();
			}
			break;
		case UNALIAS:
			remove_alias();
			break;
		case EXIT:
			break;
	}
}
void execute_command_redir(){
    int process = fork ();
    if (process > 0){
			wait ((int *) 0);
    }else if (process == 0){

    	if(cmdtab.cmd[currcmd].errfd > -1 ){
			dup2(cmdtab.cmd[currcmd].errfd, STDOUT_FILENO);
			close(cmdtab.cmd[currcmd].errfd);
		}
    	
		if(cmdtab.cmd[currcmd].outfd > -1 ){
			dup2(cmdtab.cmd[currcmd].outfd, STDOUT_FILENO);
			close(cmdtab.cmd[currcmd].outfd);
		}

   		cmdtab.cmd[currcmd].atptr->args[0]= cmdtab.cmd[currcmd].cmdname;
    	execvp( cmdtab.cmd[currcmd].cmdname, cmdtab.cmd[currcmd].atptr->args );
    	fprintf (stderr, "Can't execute \n");
    	exit (1);
    }else if(process == -1){
    	fprintf (stderr, "Can't fork!\n");
    	exit (2);
    }else{
    	printf("Syntax Error\n");
    }
}

void execute_command(){
    int process = fork ();
    if (process > 0){
			wait ((int *) 0);
    }else if (process == 0){
   		cmdtab.cmd[currcmd].atptr->args[0]= cmdtab.cmd[currcmd].cmdname;
    	execvp( cmdtab.cmd[currcmd].cmdname, cmdtab.cmd[currcmd].atptr->args );
    	fprintf (stderr, "Can't execute \n");
    	exit (1);
    }else if(process == -1){
    	fprintf (stderr, "Can't fork!\n");
    	exit (2);
    }else{
    	printf("Syntax Error\n");
    }
}

void piped_and_sniped(){

		currcmd = 0;//set to first cmd

		int num_pipes = hasPipes;//set number of pipes detected
	    int p;
	    for(p=0; p < num_pipes; ++p){ //create pipes
	    	if(pipe(pipeHolder.pipes[p]) == -1) {
         		perror("Pipe failed"); 
          		exit(1);
        	}
	    }

        int i = 0;
        while (i  <= num_pipes){ //loop of pipe redirection

        	//printf("cmd value is: %d\n", currcmd );

        	if(fork() == 0)        // fork
        	{
        		//printf("pipe value is: %d\n", hasPipes);
        		//printf("cmd value is: %d\n", currcmd );
        		if(i == 0){
        			//printf("first fork\n");
        			close(1);
        			dup(pipeHolder.pipes[i][1]);
        		}else if(i == num_pipes){
        			//printf("last fork\n");
        			if(cmdtab.cmd[currcmd].errfd > -1 ){
						dup2(cmdtab.cmd[currcmd].errfd, STDERR_FILENO);
						
					}
			    	
					if(cmdtab.cmd[currcmd].outfd > -1 ){
						dup2(cmdtab.cmd[currcmd].outfd, STDOUT_FILENO);
						close(cmdtab.cmd[currcmd].outfd);
					}

	           	 	close(0);          //closing stdout
	            	dup(pipeHolder.pipes[i-1][0]);
	        	}else{
	        		//printf("middle fork\n");
	          		dup2(pipeHolder.pipes[i-1][0], STDIN_FILENO);
	          		dup2(pipeHolder.pipes[i][1], STDOUT_FILENO);

	            }

	            int k;
	            int j;
	            for(j=0; j < 2; ++j){
		            for(k=0; k < num_pipes ; ++k){
		            	close(pipeHolder.pipes[k][j]); //close pipes
		            }
	        	}
	        	//execute commands
	            cmdtab.cmd[currcmd].atptr->args[0]= cmdtab.cmd[currcmd].cmdname;
    			execvp( cmdtab.cmd[currcmd].cmdname, cmdtab.cmd[currcmd].atptr->args );
	            perror("execvp failed");
	            close(cmdtab.cmd[currcmd].errfd);
	            exit(1);
	        }
	        ++i;
	        ++currcmd;
	        --hasPipes;
        }

        int k, j;
        for(j=0; j < 2; ++j){
		            for(k=0; k < num_pipes ; ++k){
		            	close(pipeHolder.pipes[k][j]);
		            }
	        	}

				for(j=0; j <= num_pipes; ++j){
					wait(0);//wait after each fork
				}

}

void process_command(){
	if ( builtin ){
		execute_builtin();
		clear_args();
	}
	else{
		//this if statement is for testing pipes
		printf("calling : %s\n",cmdtab.cmd[currcmd].cmdname );

		if(hasPipes > 0){
			printf("calling piped and sniped: %s\n",cmdtab.cmd[currcmd].cmdname );
			piped_and_sniped();
			clear_args();
		}else if (cmdtab.cmd[currcmd].outfd > -1 ){
			printf("attempting to close file redirect\n");
			execute_command_redir();
		}else if (cmdtab.cmd[currcmd].errfd > -1 ){
			printf("attempting to close err redirect\n");
			execute_command_redir();
		}
		else{
		execute_command();

		printf("getting here 3");
		clear_args();
		}
	}
}

void recover_from_errors(){

}

int main( int argc, char* argv[] ) {
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
				printf("%s\n", "Smoke weed erry day...");
				exit( EXIT_SUCCESS );
				break;
			case 570:
				clear_args();
				printf("%s\n", "Boogers");
				yyparse();
				process_command();
				break;
			default:
				printf("%c ", CMD);
				break;
		}
	}
}
