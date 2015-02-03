/*
 ============================================================================
 Name        : COMP310ass1.c
 Author      : Chad Nehemiah
 Version     :1.0
 Copyright   :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_LINE 80

typedef struct hist{
	char *commands[10];
	char *input[MAX_LINE];
	int params[10];
	char letter[10];
	int head;
}history;



/**
  * setup() reads in the next command line, separating it into distinct tokens
  * using whitespace as delimiters. setup() sets the args parameter as a
  * null-terminated string.
  */
void setup(char inputBuffer[], char *args[], int *background, int *num) {
	int length, /* # of characters in the command line */
	i,			/* loop index for accessing inputBuffer array */
	start,		/* index where beginning of next command parameter is */
	ct;			/* index of where to place the next parameter into agrs[] */

	ct =0;

	/* read what the user enters on the command line */
	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);

	start = -1;
	if (length == 0)
		exit(0);				/* ^d was entered, end of user command stream */
	if (length < 0) {
		perror("error reading the command");
		exit(-1);			/* terminate with error of -1 */
	}

	/* examine every character in the inputBuffer */
	for (i=0; i<length; i++) {
		switch(inputBuffer[i]) {
			case ' ':
			case '\t' :					/* argument separators */
				if(start != -1) {
					args[ct] = &inputBuffer[start];		/* set up pointer */
					ct++;
				}
				inputBuffer[i] = '\0';		/* add a null char; make a C string */
				start = -1;
				break;

			case '\n':				/* should be the final char examined */
				if (start != -1) {
					args[ct] = &inputBuffer[start];
					ct++;
				}
				inputBuffer[i] = '\0';
				args[ct] = NULL; 	/* no more arguments to this command */
				break;

			default:
				if (start == -1) start = i;
				if (inputBuffer[i] == '&') {
					*background = 1;
					inputBuffer[i] = '\0';
				}
		}
	}
	*num = ct;
	args[ct] = NULL; 	/* just in case the input line was > 80 */
}

int main (void) {
	char inputBuffer[MAX_LINE];	/* buffer to hold the command entered */
	int background;/* equals 1 if a command is followed by '&' */
	int num;/*allows me to track # of arguments*/
	char *args[MAX_LINE/+1];	/* command line (of 80) has max of 40 arguments */
	history h;/* Object to store commands */
	h.head = 0;/* Initialize head to 0*/

	while (1) {					/* program terminates normally inside setup */
		background = 0;
		printf(" COMMAND->\n");
		setup(inputBuffer,args,&background,&num);

		if(args[0][0] != 'r')/*store commands unless argument = "r"*/
		{
			int y = 0;/*iterate through array to pass args into history*/
			if(h.head > 10)/*only store 10 commands*/
			{
				h.head = 0;
			}
			h.letter[h.head] = args[0][0];
			h.params[h.head] = num;
			while(y < num)/*Iterate through array to store commands */
			{
				h.commands[y] = (char*)malloc(sizeof(char)*80);/*allocating space for memory*/
				h.input[y] = (char*)malloc(sizeof(char)*80);/*allocating space for memory*/
				strncpy(h.commands[y],args[y],MAX_LINE);/*copying into history array*/
				strncpy(h.input[y],inputBuffer,MAX_LINE);
				y++;
			}
			h.head++;/*keeps track of the head*/
		}
		if(args[0][0] == 'r' && args[0][1] == NULL) /*History option when "r" is pressed user can execute previous command*/
		{
			int i = 0;
			while(i < 10)
			{
				if(h.letter[i] == args[1][0])
				{
					int y = 0;
					strncpy(inputBuffer,h.input[i],MAX_LINE);/*changing inputBuffer to fit command*/
					while(y < h.params[i])
					{
						args[y] = (char*)malloc(sizeof(char)*80);/*allocating space for memory*/
						strncpy(args[y],h.commands[y],MAX_LINE);
						y++;
					}
					break;
				}
				else
				{
					i++;
				}
			}
		}
		pid_t pid = fork();
		if (pid == -1) {
		  // When fork() returns -1, an error happened.
		  perror("fork failed");
		  exit(EXIT_FAILURE);
	   }
	   else if (pid == 0) {
		  // When fork() returns 0, we are in the child process.
		   if(args[0][0] == 'c'&& args[1][0] == 'd')/*for the cd command*/
		   {
		   			int executed = chdir(&args);/*storing if command was properly executed*/
		   			if (executed == -1)/*command was erroneous*/
		   			{
		   				_exit(EXIT_FAILURE);
		   			}
		   			else/*command was properly executed*/
		   			{
		   				_exit(EXIT_SUCCESS);
		   			}
			}
		   if(args[0][0] == 'p'&& args[1][0] == 'w' && args[2][0] == 'd')
		   {
			    char *buf;
			    int size = 100;

			    int executed = getcwd(&buf,size);
			    if (executed == -1)/*command was erroneous*/
				{
					_exit(EXIT_FAILURE);
				}
			    else/*command was properly executed*/
				{
			    	printf("\n%s",buf);
					_exit(EXIT_SUCCESS);
				}
		   }
		   execvp(&inputBuffer,&args);
		  _exit(EXIT_SUCCESS);  // exit() is unreliable here, so _exit must be used
	   }
	   else {
		  // When fork() returns a positive number, we are in the parent process
		  // and the return value is the PID of the newly created child process.
		  int status;
		  if(background == 0){
			  (void)waitpid(pid, &status, 0);
		  }
	   }
		/* the steps are:
			(1) fork a child process using fork()
			(2) the child process will invoke execvp()
			(3) if background ==1, the parent will wait, otherwise to the setup() function */
	}
}

