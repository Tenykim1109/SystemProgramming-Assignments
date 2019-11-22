#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define PIPE_ENDS 2
#define OPTION 2
#define OPTIONSIZ 20
#define oops(m, x) { perror(m); exit(x); }


int main(int argc, char* argv[]) {
	int pipe1[PIPE_ENDS],
	pipe2[PIPE_ENDS],
	newfd, pid;

	int i;
	int named_pipe;
	if(argc<2) {
		fprintf(stderr, "input at least one command.\n");
		exit(1);
	}

	if(argc==2) {
		execlp(argv[1], argv[1], NULL);
	}
	else if(argc==3) {
		if(pipe(pipe1)==-1)
			oops("Cannot get a pipe", 1);

		if((pid=fork())==-1) 
			oops("Cannot fork", 2);

		if(pid > 0) {
			close(pipe1[1]);
			if(dup2(pipe1[0], 0)==-1)
				oops("couldn't direct stdin", 3);
			close(pipe1[0]);
			execlp(argv[2], argv[2], NULL);
			oops(argv[2], 4);
		}
		close(pipe1[0]);
		if(dup2(pipe1[1], 1)==-1)
			oops("couldn't direct stdout", 3);
		close(pipe1[1]);
		execlp(argv[1], argv[1], NULL);
		oops(argv[1], 4);
		
	}
	else {
		if(pipe(pipe1)==-1) {
		oops("Cannot get a pipe1", 1);
		}

		if(pipe(pipe2)==-1) {
			oops("Cannot get a pipe2", 1);
		}	

		if((pid=fork())==-1) //create child process
			oops("Cannot fork", 2);

		if(pid == 0) { //for child process
			close(pipe2[0]);
			if(dup2(pipe2[1], 1)==-1)
				oops("child: couldn't direct stdout", 3);
//			close(pipe2[1]);

			pid_t pid2 = fork(); //create child of child process
		
			if(pid2==-1) {
				oops("Cannot fork", 2);
			}

			if(pid2 == 0) { //for child of child
				close(pipe1[0]);
				if(dup2(pipe1[1], 1)==-1)
					oops("child of child: couldn't redirect stdout", 3);
				close(pipe1[1]);
				execlp(argv[1], argv[1], NULL);
				oops(argv[1], 4);
			}
			close(pipe1[1]);	
			if(dup2(pipe1[0], 0)==-1)
				oops("child: couldn't redirect stdin", 3);
			close(pipe1[1]);
			execlp(argv[2], argv[2], NULL);
			oops(argv[2], 4);
			close(pipe2[1]);
		}
		//for parent
		close(pipe2[1]);
		if(dup2(pipe2[0], 0)==-1)
			oops("parent: couldn't redirect stdin", 3);
		close(pipe2[0]);
		if(argc==5) {
			execlp(argv[3], argv[3], argv[4], NULL);
			oops(argv[3], 4);
		} 
		else {
			execlp(argv[3], argv[3], NULL);
			oops(argv[3], 4);
		}
	}
}
