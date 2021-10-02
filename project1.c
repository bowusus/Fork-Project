/**
 * Program demonstrating pipes.
 * The program accepts a single command-line parameter: n0, 
 * and make sure that it’s a positive integer
 * The parent will read from the pipe, and output 
 *
 * @author Bernard Owusu Sefah
 * Date: 02/23/2021	
 * Operating System Concepts  Fork Project
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#define READ_END	0
#define WRITE_END	1

int main(int argc, char *argv[] )
{
	/* Create variables and pass argument*/
	pid_t pid;
	/* Use 2 pipes first is to send input from parent 
	second pipe is to concatenate from chold */
	int num1[2]; // to store two ends of first pipe
	int num2[2]; // to store two ends of second pipe
	int numberofsteps = 0;
	signed long long childValue;
	int status;
	char *i = argv[1];
	signed long long  parentValue = atoll(i);
	signed long long peakmax = parentValue;

	/* create the pipe */
	if (pipe(num1) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
	if (pipe(num2) == -1)
	{
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	/* now fork a child process */
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 0;
	}

	if (pid > 0) {  /* parent process */
		/* close the unused end of the pipe */
		close(num1[READ_END]);
		close(num2[WRITE_END]);

		/* write to the pipe */
		write(num1[WRITE_END], &parentValue, sizeof(parentValue));
		while(parentValue != 1 || waitpid(-1, &status, WNOHANG) == -1)
		{
			printf("%llu, ",parentValue);
			read(num2[READ_END], &parentValue, sizeof(parentValue));
			numberofsteps++;
			if(parentValue > peakmax)
			{
				peakmax = parentValue;
			}
		}
		   printf("1 (%i,%llu)\n",numberofsteps,peakmax);
		
		/* close the write end of the pipe */
		close(num1[WRITE_END]);
		close(num2[READ_END]);
	}
	else { /* child process */
		/* close the unused end of the pipe */
		close(num1[WRITE_END]);
		close(num2[READ_END]);

		/* read from the pipe */
		read(num1[READ_END], &childValue, sizeof(childValue));
		while(childValue != 1)
		{	/*when the value is even*/
			if(childValue % 2 == 0)
			{	/*half the number to even*/
				childValue /=2;
			}
		else
		{	/* check number if it is odd multiply childvalue by 3 and add 1*/
			childValue = (childValue*3) +1;
		};	/*write one to the pipe*/
			write(num2[WRITE_END], &childValue, sizeof(childValue));
		}
		
	

		/* close the write end of the pipe */
		close(num1[READ_END]);
		close(num2[WRITE_END]);
	}

	return 0;

}

