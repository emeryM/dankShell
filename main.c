/*-------------------------------------------------------------------
 * main.c : the main loop of dankShell
 *-------------------------------------------------------------------
 */


#include "dank.h"
#include "y.tab.h"


void shell_init(){
	int i;
	//initialize command table and variables
	for (i= 0; i < MAXCMDS; ++i){
		ARGTAB *args = malloc(sizeof(ARGTAB));
		COMMAND cmd;
		cmd.cmdname = NULL;
		cmd.infd = -1;
		cmd.outfd = -1;
		cmd.errfd = -1;
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
		pipetab.pipes[i] = arr;
	}
	has_pipes = 0;
	//initialize alias table and variables
	char alstring[MAXINPUTLENGTH] = {0};
	alias.reparse_string = alstring;
	alias.used = 0;
	alias_detected = 0;
	loop_detected = 0;
	print_flag = 0;
}


void print_prompt(){
	if( print_flag == 0 ){
		printf(KGRN "dankShell:");
		printf(KBLK " ");
	}
}


void clear_args(){
	int i;
	int j;
	//clear command table and variables
	for(currcmd = 0; currcmd <= cmdcount; ++currcmd){
		cmdtab.cmd[currcmd].cmdname = NULL;
		cmdtab.cmd[currcmd].infd = -1;
		cmdtab.cmd[currcmd].outfd = -1;
		cmdtab.cmd[currcmd].errfd = -1;
		for (i = 0; i < cmdtab.cmd[currcmd].nargs+1; ++i){
			cmdtab.cmd[currcmd].atptr->args[i] = NULL;
		}
		cmdtab.cmd[currcmd].nargs = 0;
	}
	currcmd = 0;
	cmdcount = 0;
	builtin = 0;
	//clear pipe variables
	has_pipes = 0;
	//clear alias variables
	alias_detected = 0;
	memset(alias.reparse_string,0,strlen(alias.reparse_string));
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
		printf("No aliases to display.\n");
	}
	else{
		int i;
		for(i = 0; i < alias.used; ++i ){
			printf("%s\t%s\n", alias.alname[i], alias.alstr[i]);
		}
	}
}


void set_alias(){
	if (alias.used >= MAXALIAS){
		printf("Too many aliases already exist.\n");
	}
	else{
		int i = 0;
		while( i < alias.used && strcmp(cmdtab.cmd[currcmd].atptr->args[0], alias.alname[i]) ){
			++i;
		}
		alias.alname[i] = cmdtab.cmd[currcmd].atptr->args[0];
		alias.alstr[i] = cmdtab.cmd[currcmd].atptr->args[1];
		alias.found[i] = 0;
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
		fprintf(stderr, "Error: Alias does not exist.");
	}
	else{
		while( i < alias.used ){
			alias.alname[i] = alias.alname[i+1];
			alias.alstr[i] = alias.alstr[i+1];
			++i;
		}
		alias.alname[alias.used] = "";
		alias.found[alias.used] = 0;
		alias.alstr[alias.used--] = "";
		printf("Alias removed.\n");
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
	if(process == -1){
		perror("Fork error");
		exit (2);
	}
	else if (process == 0){
		if(cmdtab.cmd[currcmd].infd > -1 ){
			dup2(cmdtab.cmd[currcmd].infd, STDIN_FILENO);
			close(cmdtab.cmd[currcmd].infd);
		}
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
		perror("Execvp error");
		exit (1);
	}
	else if (process > 0){
		wait ((int *) 0);
	}
	else{
		fprintf(stderr, "Syntax Error\n");
	}
}


void execute_command(){
	int process = fork ();
	if(process == -1){
		perror("Fork error");
		exit (2);
	}
	else if (process == 0){
		cmdtab.cmd[currcmd].atptr->args[0]= cmdtab.cmd[currcmd].cmdname;
		execvp( cmdtab.cmd[currcmd].cmdname, cmdtab.cmd[currcmd].atptr->args );
		perror("Execvp error");
		exit (1);
	}
	else if (process > 0){
		wait ((int *) 0);
	}
	else{
		fprintf(stderr, "Syntax Error.\n");
	}
}


void piped_and_sniped(){
	// begin with first command
	currcmd = 0;

	// detect number of pipes required
	int num_pipes = has_pipes;

	//create necessary pipes
	int p;
	for(p=0; p < num_pipes; ++p){
		if(pipe(pipetab.pipes[p]) == -1){
			fprintf(stderr, "Pipe error\n");
			exit(1);
		}
	}

	//loop pipe redirection
	int i = 0;
	while (i  <= num_pipes){
		// fork
		if(fork() == 0){
			if(cmdtab.cmd[currcmd].infd > -1 ){
				dup2(cmdtab.cmd[currcmd].infd, STDIN_FILENO);
				close(cmdtab.cmd[currcmd].infd);
			}
			// special case: first fork
			if(i == 0){
				close(1);
				dup(pipetab.pipes[i][1]);
			}
			// special case: last fork
			else if(i == num_pipes){
				if(cmdtab.cmd[currcmd].errfd > -1 ){
					dup2(cmdtab.cmd[currcmd].errfd, STDERR_FILENO);
				}
				if(cmdtab.cmd[currcmd].outfd > -1 ){
					dup2(cmdtab.cmd[currcmd].outfd, STDOUT_FILENO);
					close(cmdtab.cmd[currcmd].outfd);
				}
				// closing stdout
				close(0);
				dup(pipetab.pipes[i-1][0]);
			}
			// case: any middle fork
			else{
				dup2(pipetab.pipes[i-1][0], STDIN_FILENO);
				dup2(pipetab.pipes[i][1], STDOUT_FILENO);
			}
			// close
			int k;
			int j;
			for(j=0; j < 2; ++j){
				for(k=0; k < num_pipes ; ++k){
					close(pipetab.pipes[k][j]);
				}
			}
			//execute command
			cmdtab.cmd[currcmd].atptr->args[0]= cmdtab.cmd[currcmd].cmdname;
			execvp( cmdtab.cmd[currcmd].cmdname, cmdtab.cmd[currcmd].atptr->args );
			perror("Execvp error");
			close(cmdtab.cmd[currcmd].errfd);
			exit(1);
		}
		++i;
		++currcmd;
		--has_pipes;
	}

	// close pipes
	int k, j;
	for(j=0; j < 2; ++j){
		for(k=0; k < num_pipes ; ++k){
			close(pipetab.pipes[k][j]);
		}
	}

	// wait once per pipe
	for(j=0; j <= num_pipes; ++j){
		wait(0);
	}
}


void process_command(){
	if( loop_detected > 0 ){
		loop_detected = 0;
		int i;
		for( i = 0; i < alias.used; ++i ){
			alias.found[i] = 0;
		}
		fprintf(stderr, "Error: Infinite alias loop detected.\n");
	}
	else if ( builtin ){
		execute_builtin();
	}
	else{
		if(has_pipes > 0){
			piped_and_sniped();
			clear_args();
		}
		else if (cmdtab.cmd[currcmd].infd > -1 ){
			execute_command_redir();
		}
		else if (cmdtab.cmd[currcmd].outfd > -1 ){
			execute_command_redir();
		}
		else if (cmdtab.cmd[currcmd].errfd > -1 ){
			execute_command_redir();
		}
		else
		{
			execute_command();
		}
	}
	clear_args();
	int i;
	for ( i = 0; i < alias.used; ++i ){
		alias.found[i] = 0;
	}
	print_flag = 0;
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
				clear_args();
				recover_from_errors();
				break;
			case 2:
				printf("%s\n", "Succeed erry day...");
				exit( EXIT_SUCCESS );
				break;
			case 3:
				clear_args();
				yyparse();
				process_command();
				break;
			default:
				fprintf(stderr, "Error: Unable to get command.\n");
				break;
		}
	}
}
