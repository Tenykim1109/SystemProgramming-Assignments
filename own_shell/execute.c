#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "smsh.h"

int execute(char* argv[]) {
	int pid;
	int child_info = -1;
	int i;

	if((pid=fork())==-1)
		perror("fork");
	else if(pid==0) {
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if(option=='&') {
			printf("PID : %d\n", getpid());
			execvp(argv[0], argv);
		}
		else {
			execvp(argv[0], argv);
		}
	}
	else if(pid!=0 && option==0) {
		if(wait(&child_info)==-1)
			perror("wait");
	}
	return child_info;
}


