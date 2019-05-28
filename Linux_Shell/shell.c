// Shell starter file
// You may make any changes to any part of this file.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)



/**
 * Command Input and Processing
 */

/*
 * Tokenize the string in 'buff' into 'tokens'.
 * buff: Character array containing string to tokenize.
 *   	Will be modified: all whitespace replaced with '\0'
 * tokens: array of pointers of size at least COMMAND_LENGTH/2 + 1.
 *   	Will be modified so tokens[i] points to the i'th token
 *   	in the string buff. All returned tokens will be non-empty.
 *   	NOTE: pointers in tokens[] will all point into buff!
 *   	Ends with a null pointer.
 * returns: number of tokens.
 */
int tokenize_command(char *buff, char *tokens[])
{
	int token_count = 0;
	_Bool in_token = false;
	int num_chars = strnlen(buff, COMMAND_LENGTH);
	for (int i = 0; i < num_chars; i++) {
    	switch (buff[i]) {
    	// Handle token delimiters (ends):
    	case ' ':
    	case '\t':
    	case '\n':
        	buff[i] = '\0';
        	in_token = false;
        	break;

    	// Handle other characters (may be start)
    	default:
        	if (!in_token) {
            	tokens[token_count] = &buff[i];
            	token_count++;
            	in_token = true;
        	}
    	}
	}
	tokens[token_count] = NULL;
	return token_count;
}

#define HISTORY_DEPTH 10
char history[HISTORY_DEPTH][COMMAND_LENGTH];
int numcommands = 0;

void history_store_command(char *new_command)
{
	char temp1[1024];
	char temp2[1024];
	if(numcommands > 9){
  	memcpy(temp1, history[9], 1024);
  	for(int i = 9; i > 0; i--){
  	memcpy(temp2, history[i - 1], 1024);
  	memcpy(history[i - 1], temp1, 1024);
  	memcpy(temp1, temp2, 1024);
  	}
  	memcpy(history[9], new_command, COMMAND_LENGTH);
	}
	else{
  	memcpy(history[numcommands], new_command, COMMAND_LENGTH);
	}
	numcommands++;
}



void handle_SIGINT(){
	write(STDOUT_FILENO, "\n", strlen("\n"));
	write(STDOUT_FILENO, "exit: Exit the shell program\n", strlen("exit: Exit the shell program\n"));
    write(STDOUT_FILENO, "pwd: Display the current working directory\n", strlen("pwd: Display the current working directory\n"));
    write(STDOUT_FILENO, "cd: Change the current working directory\n", strlen("cd: Change the current working directory\n"));
	char buff[1024];
    getcwd(buff, 1024);
    write(STDOUT_FILENO, buff, strlen(buff));
    write(STDOUT_FILENO, "$ ", strlen("$ "));
}

_Bool intcommands(char *tokens[NUM_TOKENS]){
	if(strcmp(tokens[0],"exit") == 0){
		if(tokens[1] != 0){
        	write(STDOUT_FILENO, "Too much arguments\n", strlen("Too much arguments\n"));
        	exit(-1);
    	}
    	exit(0);
	}
	else if(strcmp(tokens[0],"history") == 0){
		if(tokens[1] != 0){
        	write(STDOUT_FILENO, "Too much arguments\n", strlen("Too much arguments\n"));
        	exit(-1);
    	}
    	if(numcommands < 10){
		for(int i = numcommands - 1; i > 0; i--)
		{
		char buffer[1024];
		snprintf(buffer,1024,"%d",i);
		write(STDOUT_FILENO, buffer, strlen(buffer));
		write(STDOUT_FILENO, " ", strlen(" "));
      	write(STDOUT_FILENO, history[i], strlen(history[i]));
      	write(STDOUT_FILENO,"\n", strlen("\n"));
    	}
		}
		else{
		int t = 0;
		for(int i = 9; i >= 0; i--)
		{
			int num = numcommands - t;
			char buffer[1024];
			snprintf(buffer,1024,"%d",num);
			write(STDOUT_FILENO, buffer, strlen(buffer));
			write(STDOUT_FILENO, " ", strlen(" "));
			write(STDOUT_FILENO, history[i], strlen(history[i]));
			write(STDOUT_FILENO,"\n", strlen("\n"));
			t = t + 1;
    	}
	}

	}
	else if(strcmp(tokens[0],"pwd") == 0){
    	if(tokens[1] != 0){
        	write(STDOUT_FILENO, "Too much arguments\n", strlen("Too much arguments\n"));
        	exit(-1);
    	}
    	char buffer[1024];
    	getcwd(buffer, 1024);
    	write(STDOUT_FILENO, buffer, strlen(buffer));
		write(STDOUT_FILENO, "\n", strlen("\n"));
	}

	else if(strcmp(tokens[0],"cd") == 0){
    	if(tokens[2] != 0){
        	write(STDOUT_FILENO, "Too much arguments\n", strlen("Too much arguments\n"));
        	exit(-1);
    	}
    	int ret = chdir(tokens[1]);
    	if(ret != 0){
        	write(STDOUT_FILENO, "Invalid Directory", strlen("Invalid Directory"));
    	}
		
	}
	else if(strcmp(tokens[0],"help") == 0){
		if((tokens[1] != 0)){
			if(tokens[2] != 0){
				write(STDOUT_FILENO, "Too much arguments\n", strlen("Too much arguments\n"));
			
			}
			else if((strcmp(tokens[1],"exit") == 0)){
				write(STDOUT_FILENO, "exit: Exit the shell program\n", strlen("exit: Exit the shell program\n"));
			
			}
			else if((strcmp(tokens[1],"pwd") == 0)){
				write(STDOUT_FILENO, "pwd: Display the current working directory\n", strlen("pwd: Display the current working directory\n"));
			
			}
			else if((strcmp(tokens[1],"cd") == 0)){
				write(STDOUT_FILENO, "cd: Change the current working directory\n", strlen("cd: Change the current working directory\n"));
			
			}
			else if(tokens[1] != 0){
				write(STDOUT_FILENO, tokens[1], strlen(tokens[1]));
				write(STDOUT_FILENO, " ", strlen(" "));
				write(STDOUT_FILENO, "is an external command or application\n", strlen("is an external command or application\n"));
			}
		}
		else{
    	write(STDOUT_FILENO, "exit: Exit the shell program\n", strlen("exit: Exit the shell program\n"));
    	write(STDOUT_FILENO, "pwd: Display the current working directory\n", strlen("pwd: Display the current working directory\n"));
    	write(STDOUT_FILENO, "cd: Change the current working directory\n", strlen("cd: Change the current working directory\n"));
		}
		
		
	}
	else{
    	return false;
	}

	return true;
}

/**
 * Read a command from the keyboard into the buffer 'buff' and tokenize it
 * such that 'tokens[i]' points into 'buff' to the i'th token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 *   	COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into 'buff'. Must be at
 *   	least NUM_TOKENS long. Will strip out up to one final '&' token.
 *   	tokens will be NULL terminated (a NULL pointer indicates end of tokens).
 * in_background: pointer to a boolean variable. Set to true if user entered
 *   	an & as their last token; otherwise set to false.
 */
void read_command(char *buff, char *tokens[], _Bool *in_background)
{
	*in_background = false;
	char stored[COMMAND_LENGTH];

	// Read input

	int length = -1;
	while (length < 0) {
    	length = read(STDIN_FILENO, stored, COMMAND_LENGTH-1);
	}

	stored[length] = '\0';
	int newline = length - 1;
	if (stored[newline] == '\n') {
    	stored[newline] = '\0';
	}
	
	if (length < 0) {
    	perror("Unable to read command from keyboard. Terminating.\n");
    	exit(-1);
	}

	memcpy(buff, stored, COMMAND_LENGTH);
	
	
	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
    	return;
	}
	
	if (tokens[0][0] == '!'){
		int histnum = 0;
		if (tokens[0][1] == '!') {
			histnum = numcommands;
 		} else {
 			histnum = atoi(&tokens[0][1]);
 		}
		int commandgetnum = histnum - numcommands;
		if(abs(commandgetnum) <= 10){
			char commandbuffer[1024];
			memcpy(commandbuffer, history[commandgetnum + 9], 1024);
			memcpy(stored, history[commandgetnum + 9], 1024);
			write(STDOUT_FILENO, commandbuffer, strlen(commandbuffer));
			write(STDOUT_FILENO, "\n",strlen("\n"));
			token_count = tokenize_command(commandbuffer, tokens);
		}
		else{
			write(STDOUT_FILENO, "Error while trying to receive command", strlen("Error while trying to receive command"));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}
		
		
		
	}

	if (token_count > 0) {
    	history_store_command(stored);
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
    	*in_background = true;
    	tokens[token_count - 1] = 0;
	}
}

/**
 * Main and Execute Commands
 */
int main(int argc, char* argv[])
{
	char input_buffer[COMMAND_LENGTH];
	char *tokens[NUM_TOKENS];

	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	handler.sa_flags = 0;
	sigemptyset(&handler.sa_mask);
	sigaction(SIGINT, &handler, NULL);



	while (true) {

    	// Cleanup any previously exited background child processes
    	// (The zombies)
    	while (waitpid(-1, NULL, WNOHANG) > 0)
        	; // do nothing.



    	// Get command
    	// Use write because we need to use read() to work with
    	// signals, and read() is incompatible with printf().

    	char buff[1024];
    	getcwd(buff, 1024);
    	write(STDOUT_FILENO, buff, strlen(buff));
    	write(STDOUT_FILENO, "$ ", strlen("$ "));
    	_Bool in_background = false;
    	read_command(input_buffer, tokens, &in_background);

    	if(tokens[0] == 0){
        	continue;
    	}

    	_Bool handle = intcommands(tokens);


    	if(handle == false){
        	pid_t pid;
        	pid = fork();
        	if(pid < 0){
            	write(STDOUT_FILENO, "Error in Child Process", strlen("Error in Child Process"));
            	exit(-1);
        	}
        	if(pid == 0){
            	execvp(tokens[0], tokens);
        	}
        	if(in_background == false){
            	waitpid(pid, NULL, 0);
        	}
    	}
    	
    	/**
     	* Steps For Basic Shell:
     	* 1. Fork a child process
     	* 2. Child process invokes execvp() using results in token array.
     	* 3. If in_background is false, parent waits for
     	*	child to finish. Otherwise, parent loops back to
     	*	read_command() again immediately.
     	*/

	}
	return 0;
}

