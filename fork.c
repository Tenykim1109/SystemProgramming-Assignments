#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sig_handler(int signum) {
	if(signum==SIGINT) {
		printf("\nsignal CTRL+C received.\n");
		signal(SIGINT, SIG_DFL);
		exit(-1);
	}
	else if(signum==SIGQUIT) {
		printf("\nsignal CTRL+\\ received.\n");
		signal(SIGQUIT, SIG_DFL);
		exit(-1);
	}
}

void child_code() {
	int x, y;
	double avg;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	printf("Input two numbers (x and y)\n");
	printf("	I am Child %d.\n", getpid());
	while(1) {
		scanf("%d %d", &x, &y);
		if(x==y) {
			printf("The program received the same numbers. So the program exits.\n");
			exit(-1);
		}
		avg=(double)(x+y)/2;
		if(avg-(int)avg==0.0)
			printf("%d\n", (int)avg);
		else
			printf("%.1lf\n", avg);
	}
	exit(17);
}

void parent_code(int childpid) {
	int wait_rv, high_8;
	int child_status;
	wait_rv=wait(&child_status);
	high_8=child_status>>8;
	printf("	Child %d dead. :(\n", childpid);
	printf("		exit code=%d\n", high_8);
}

int main(int argc, char *argv[]) {
	int newpid;
	void child_node(), parent_node();

	printf("	I am parent %d\n", getpid());

	if((newpid=fork())==-1) 
		perror("fork");
	else if (newpid==0) 
		child_code();
	else
		parent_code(newpid);
}
