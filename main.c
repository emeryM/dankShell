/*-------------------------------------------------------------------
 * main.c : the main loop of dankShell
 *-------------------------------------------------------------------
 */

#include "dank.h"
#include "y.tab.h"

void shell_init(){
	int i;
	for (i= 0; i < MAXCMDS; ++i){
		ARGTAB *args = malloc(sizeof(ARGTAB));
		COMMAND cmd;
		cmd.atptr = args;
		cmdtab.cmd[i] = cmd;
	}
	for(i = 0; i < MAXPIPES; ++i){
		int* arr = (int*)malloc(2 * sizeof(int));
		pipeHolder.pipes[i] = arr;
	}
	alias.used = 0;
	alias_detected = 0;
	currcmd = 0;
	cmdcount = 0;
	hasPipes = 0;
}

void print_prompt(){
	printf(KGRN "\ndankShell:");
	printf(KBLK " ");
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


void clear_args(){
	int i;
	int j;

	for(currcmd = 0; currcmd <= cmdcount; ++currcmd){
		//printf("clearing: %s\n",cmdtab.cmd[currcmd].cmdname);
		for (i = 0; i < cmdtab.cmd[currcmd].nargs+1; ++i)
		{
			cmdtab.cmd[currcmd].atptr->args[i] = NULL;
		}
		cmdtab.cmd[currcmd].nargs = 0;
		cmdtab.cmd[currcmd].infd = 0;
		cmdtab.cmd[currcmd].outfd = 0;
		cmdtab.cmd[currcmd].cmdname = NULL;
	}
	currcmd = 0;
	hasPipes = 0;
	cmdcount = 0;

}

void piped_and_sniped(){

		//temp
		currcmd = 0;
		//int pipeHolder[100][2];

		//do maht here to set the size of the des_p array
		int num_pipes = hasPipes;//set it
	   // int des_p[2];// array of pid's for pipe destinations
	    int p;
	    for(p=0; p < num_pipes; ++p){
	    	if(pipe(pipeHolder.pipes[p]) == -1) {
         		perror("Pipe failed");
          		exit(1);
        	}
	    }
        /*if(pipe(des_p) == -1) {
          perror("Pipe failed");
          exit(1);
        }*/
        int i = 0;

       // hasPipes+=1;
        while (i  <= num_pipes){

        	printf("cmd value is: %d\n", currcmd );

        	if(fork() == 0)        //first fork
        	{

        		printf("pipe value is: %d\n", hasPipes);
        		printf("cmd value is: %d\n", currcmd );
        		if(i == 0){
        			printf("first fork\n");
        			close(1);
        			dup(pipeHolder.pipes[i][1]);
        		}else if(i == num_pipes){
        			printf("last fork\n");
	           	 	close(0);          //closing stdout
	            	dup(pipeHolder.pipes[i-1][0]);
	        	}else{
	        		printf("middle fork\n");
	            	//close(1);
	            	//close(0);
	        		//dup(pipeHolder.pipes[i][1]);     //replacing stdout with pipe write
	          		//dup(pipeHolder.pipes[i-1][0]);
	          		dup2(pipeHolder.pipes[i-1][0], STDIN_FILENO);
	          		dup2(pipeHolder.pipes[i][1], STDOUT_FILENO);

	            }

	            //close(des_p[0]);   //closing pipe read
	            //close(des_p[1]);
	            int k;
	            int j;
	            for(j=0; j < 2; ++j){
		            for(k=0; k < num_pipes ; ++k){
		            	close(pipeHolder.pipes[k][j]);
		            }
	        	}
	            /*for(k=0; k<2; k++)
	        		{close(des_p[k]);//close(bc[i]); close(ca[i]); }
	        	}*/


	            //const char* prog1[] = { "ls", "-l", 0};
	            //execvp(prog1[0], prog1);
	            cmdtab.cmd[currcmd].atptr->args[0]= cmdtab.cmd[currcmd].cmdname;
    			execvp( cmdtab.cmd[currcmd].cmdname, cmdtab.cmd[currcmd].atptr->args );
	            perror("execvp of ls failed");
	            exit(1);
	        }
	        ++i;
	        ++currcmd;
	        --hasPipes;
        }

       // close(des_p[0]);
        //close(des_p[1]);
        int k, j;
        for(j=0; j < 2; ++j){
		            for(k=0; k < num_pipes ; ++k){
		            	close(pipeHolder.pipes[k][j]);
		            }
	        	}

				for(j=0; j <= num_pipes; ++j){
					wait(0);
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
		}
		else{
		execute_command();
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
			default:
				printf("%c ", CMD);
				break;
		}
	}
}
