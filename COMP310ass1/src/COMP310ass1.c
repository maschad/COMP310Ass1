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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_LINE 80

typedef struct node{
	char arg [];
	char command;
	char letter;
	int length;
	struct node *next;
	struct node *prev;
}node_t;


void create(node_t * current, char * args[80])
{
	node_t * temp = malloc(sizeof(node_t));
	current->next = temp;
	temp->prev = current;
	char *new= malloc(sizeof(args[0]));
	strcpy(new, args[0]);
	temp->command = new;
	temp->letter= (new)[0];
	int k=0;
	while(1)
	{
		if (args[k] == NULL)
		{
			temp->length = k-1; break;
		}
		char *tera= malloc( sizeof(args[k]));
		strcpy(tera, args[k]);
		temp->arg[k]= tera;
		k++;
	}
	temp->next = NULL;
}


/**
  * setup() reads in the next command line, separating it into distinct tokens
  * using whitespace as delimiters. setup() sets the args parameter as a
  * null-terminated string.
  */
int setup(char inputBuffer[], char *args[], int *background) {
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
	node_t commands[10][100];/* Array to store last 10 commands*/
	int letter,count = 0; /*keeps track of the commands*/
	char inputBuffer[MAX_LINE];	/* buffer to hold the command entered */
	int background;				/* equals 1 if a command is followed by '&' */
	char *args[MAX_LINE/+1];	/* command line (of 80) has max of 40 arguments */
	pid_t pid;

	while (1) {					/* program terminates normally inside setup */
		background = 0;
		printf(" COMMAND->\n");
		setup(inputBuffer, args, &background);
		letter = 0;
		while(letter < 80 && inputBuffer[letter]== "/n")
		{
			letter++;
		}
		count++;

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
