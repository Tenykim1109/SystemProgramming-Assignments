/*SIGINT - CTRL+C
 SIGQUIT - CTRL+\*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>

#define BUFSIZE 256

int count=0;

int set_crmode() {
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag ^= ICANON;
	ttystate.c_cc[VMIN]=1;
	tcsetattr(0, TCSANOW, &ttystate);
} 

int tty_mode(int how) {
	static struct termios original_mode;
	if(how==0) {
		return tcgetattr(0, &original_mode);
	}
	else {
		return tcsetattr(0, TCSANOW, &original_mode);
	}
}

void sig_handler(int signum) {
	char question[BUFSIZE];
	char res;

	if (signum==SIGINT)
		sprintf(question, "%s! Interrupted! OK to quit (y/n)?", "CTRL+C");
	else if(signum==SIGQUIT)
		sprintf(question, "%s! Interrupted! OK to quit (y/n)?", "CTRL+\\");

	printf("%s", question);
	while(1) {
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		res=getchar();
		if(res=='y') {
			printf("\nOkay, Bye...\n");
			tty_mode(1);
			exit(1);
		}
		else if(res=='n') {
			printf("\nHello printed %d times\n", count);
			count=0;
			break;
		}
		else {
			printf("\ncannot understand %c, Please type y or n. :(\n", res);
		}
	}
}

int main() {
	int i;
	
	tty_mode(0);
	set_crmode();
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	for(i=0; i<12; i++) {
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, sig_handler);
		++count;
		printf("hello %d\n", i+1);
		sleep(1);
	}
	tty_mode(1);
}


