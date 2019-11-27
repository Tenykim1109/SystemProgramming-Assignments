#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define oops(msg) { perror(msg); exit(1); }
#define BUFF_SIZE 100

FILE *popen2(const char*, const char*);
void sig_handler(int signum) {
	if(signum==SIGINT)
		printf("	SIGINT received but ignored...\n");
	if(signum==SIGQUIT)
		printf("	SIGQUIT received but ignored...\n");
}

int main() {
	FILE *pipein_fp, *pipeout_fp;
	char readbuf[BUFF_SIZE];

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);

	if((pipein_fp = popen2("ls", "r"))==NULL) 
		oops("input: popen2");

	if((pipeout_fp = popen2("sort -r", "w"))==NULL) 
		oops("output: popen2");

	while(fgets(readbuf, BUFF_SIZE, pipein_fp))
		fputs(readbuf, pipeout_fp);

	pclose(pipein_fp);
	pclose(pipeout_fp);
	sleep(1);

	return 0;
}

FILE *popen2(const char *command, const char *mode) {
	pid_t pid;
	FILE *fp;
	int mypipe[2];
	int parent_end, child_end;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);

	if(strcmp(mode, "r")==0) {
		parent_end=1;
		child_end=0;
	}

	if(strcmp(mode, "w")==0) {
		parent_end=0;
		child_end=1;
	}
	
	if(pipe(mypipe)==-1) 
		oops("pipe");

	if((pid=fork())==-1)
		oops("cannot fork");

	if(pid > 0) { 
		close(mypipe[parent_end]);
		fp = fdopen(mypipe[child_end], mode);
	}

	if(pid == 0) {
		close(mypipe[child_end]);
		if(dup2(mypipe[parent_end], parent_end)==-1) 
			oops("Cannot redirect");
		close(mypipe[parent_end]);
		execl("/bin/sh", "sh", "-c", command, NULL);
		oops(command);
	}

	return fp;
}
