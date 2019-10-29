#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#define BLANK "	"

char symbol;
int control_char;
int top_row;
int bottom_row;
int leftedge;
int rightedge;
int row;
int col;
int dir_x;
int dir_y;

int set_ticker(int n_msecs) {
	struct itimerval new_timeset; 
	long n_sec, n_usecs;

	n_sec=n_msecs/1000;
	n_usecs=(n_msecs%1000)*1000L;

	new_timeset.it_interval.tv_sec=n_sec;
	new_timeset.it_interval.tv_usec=n_usecs;
	new_timeset.it_value.tv_sec=n_sec;
	new_timeset.it_value.tv_usec=n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void move_msg(int signum) {
	signal(SIGALRM, move_msg);
	move(row, col);
	addstr(BLANK);
	col+=dir_x;
	row+=dir_y;
	move(row, col);
	addstr(&symbol);
	refresh();

	if(dir_x<0 && col<=leftedge)
		dir_x*=-1;
	else if(dir_y<0 && row<=top_row)
		dir_y*=-1;
	else if(dir_x>0 && col+1>=rightedge)
		dir_x*=-1;
	else if (dir_y>0 && row+1>=bottom_row)
		dir_y*=-1;
}

int main(int argc, char *argv[]) {
	int delay;
	int ndelay;
	void move_msg(int);

	if(argc!=8) {
		printf("Usage: command symbol start_col start_row top_row bottom_row l_edge r_edge\n");
		exit(1);
	}

	initscr();
	crmode();
	noecho();
	clear();

	symbol=*argv[1];
	row=atoi(argv[3]);
	col=atoi(argv[2]);
	top_row=atoi(argv[4]);
	bottom_row=atoi(argv[5]);
	leftedge=atoi(argv[6]);
	rightedge=atoi(argv[7]);
	dir_x=1;
	dir_y=1;
	delay=1000;

	move(row, col);
	addstr(&symbol);
	signal(SIGINT, SIG_IGN);
	signal(SIGALRM, move_msg);
	set_ticker(delay);

	while(true) {
		ndelay=0;
		control_char=getch();
		if(control_char=='Q') break; //quit

		if(ndelay>0)
			set_ticker(delay=ndelay);

		if(control_char=='e')
			dir_x/=3;
		else if(control_char=='c')
			dir_y/=3;
		else if(control_char=='r')
			dir_x*=3;
		else if(control_char=='v')
			dir_y*=3;
	}
	endwin();
	return 0;
}

