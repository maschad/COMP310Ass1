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
#include <unistd.h>
#define MAX_LINE 80

typedef struct node{
	char *command[MAX_LINE/+1];
	char letter;
	struct node *next;
	struct node *prev;
}anode;



/**
  * setup() reads in the next command line, separating it into distinct tokens
  * using whitespace as delimiters. setup() sets the args parameter as a
  * null-terminated string.
  */
void setup(char inputBuffer[], char *args[], int *background) {
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
	args[ct] = NULL; 	/* just in case the input line was > 80 */
}

int main (void) {
	anode *history,*start;/* Array to store last 10 commands*/
	int count = 0; /*keeps track of the commands*/
	char inputBuffer[MAX_LINE];	/* buffer to hold the command entered */
	int background;				/* equals 1 if a command is followed by '&' */
	char *args[MAX_LINE/+1];	/* command line (of 80) has max of 40 arguments */
	pid_t pid;

	start = (struct node *)malloc(sizeof(struct node));/*allocating space for memory*/
	history = start;
	start->next = NULL;

	while (1) {					/* program terminates normally inside setup */
		background = 0;
		printf(" COMMAND->\n");
		setup(inputBuffer, args, &background);

		if(args[0][0] != 'r')/*store commands unless argument = "r"*/
		{
			history->command[0] = args[0];/*storing the command*/
			history->letter = args[0][0];/*storing first letter of the command*/
			count++;/*increment the count of executed commands*/
			history->next = (struct node *)malloc(sizeof(struct node));
			history->prev = history;
			history->next = NULL;
		}
		if(args[0][0] == 'r' && args[0][1] == NULL)/*History option when "r" is pressed user can execute previous command*/
		{
			anode *current;
			current = (struct node *)malloc(sizeof(struct node));
			current = history;
			int i = 0;
			while(current->prev != NULL && i < 10)
			{
				if(args[1][0] == current->letter)
				{
					args[0] = current->command[0];
					break;
				}
				else
				{
					current->prev = history->prev;
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
