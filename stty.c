/*주의사항
igncr값을 off하면 carrige return 값 무시 - enter키가 안 먹히므로
명령어를 입력하고 ctrl+j키를 눌러 명령어 실행할 것
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>

/*flaginfo 구조체 - fl_value: flag 설정값; fl_name: flag 이름*/
struct flaginfo {
	int fl_value;
	char *fl_name;
};

struct flaginfo input_flags[] = {
	IGNBRK, "ignbrk",
	BRKINT, "brkint",
	IGNPAR, "ignpar",
	PARMRK, "parmrk",
	INPCK, "inpck",
	ISTRIP, "istrip",
	INLCR, "inlcr",
	IGNCR, "igncr",
	ICRNL, "icrnl",
	IXON, "ixon",
	IXOFF, "ixoff",
	0, NULL};

struct flaginfo output_flags[] = {
	OLCUC, "olcuc",
	0, NULL};

struct flaginfo local_flags[] = {
	ISIG, "isig",
	ICANON, "icanon",
	ECHO, "echo",
	ECHOE, "echoe",
	ECHOK, "echok",
	0, NULL};

int showbaud(int thespeed);
void set_flags_flip(int fd, struct termios ttyp);
void set_special_flags(int fd, struct termios ttyp);
void show_some_flags(struct termios *ttyp);
void show_flagset(int thevalue, struct flaginfo thebitnames[]);

int main(int argc, char *argv[]) {
	int fd = STDOUT_FILENO;
	struct termios ttyinfo; //터미널 설정 정보를 담고있는 구조체

	if(tcgetattr(0, &ttyinfo)==-1) {
		perror("cannot get params about stdin");
		exit(1);
	}

	if(argc==2) {
		if(strcmp(argv[1], "-E")==0) {
			set_flags_flip(fd, ttyinfo); //현재 설정된 tty값을 뒤집음
		}
		else if(strcmp(argv[1], "-S")==0) {
			set_special_flags(fd, ttyinfo); //특수 tty값만 뒤집음
		}
	}

	printf("speed %d baud;\n \n", showbaud(cfgetospeed(&ttyinfo)));
	printf("erase = ^%c;  ", ttyinfo.c_cc[VERASE]-1);
	printf("kill = ^%c;  ", ttyinfo.c_cc[VKILL]-1+'A');
	printf("\n \n");
	show_some_flags(&ttyinfo);
}

int showbaud(int thespeed) {
	switch(thespeed) {
		case(B50): return 50;
		case(B75): return 75;
		case(B110): return 110;
		case(B134): return 134;
		case(B150): return 150;
		case(B200): return 200;
		case(B300): return 300;
		case(B600): return 600;
		case(B1200): return 1200;
		case(B2400): return 2400;
		case(B4800): return 4800;
		case(B9600): return 9600;
		case(B19200): return 19200;
		case(B38400): return 38400;
		case(B115200): return 115200;
		default: return 0;
	}
}

/*구조체에 설정된 stty값 모두 뒤집기*/
void set_flags_flip(int fd, struct termios ttyp) {
	int i;

	for(i=0; input_flags[i].fl_value ; i++) {
		ttyp.c_iflag^=input_flags[i].fl_value; //모든 input flag를 뒤집는다
	}

	for(i=0; output_flags[i].fl_value ; i++) {
		ttyp.c_oflag^=output_flags[i].fl_value;
	}

	for(i=0; local_flags[i].fl_value ; i++) {
		ttyp.c_lflag^=local_flags[i].fl_value;
	}

	tcsetattr(fd, TCSANOW, &ttyp);

}

/*특정 stty값 뒤집기*/
void set_special_flags(int fd, struct termios ttyp) {
	int i;

	for(i=0; input_flags[i].fl_value; i++) {
		if(!strcmp(input_flags[i].fl_name, "ignbrk"))
			ttyp.c_iflag^=input_flags[i].fl_value;
	}

	for(i=0; output_flags[i].fl_value; i++) {
		if(!strcmp(output_flags[i].fl_name, "olcuc"))
			ttyp.c_oflag^=output_flags[i].fl_value;
	}

	for(i=0; local_flags[i].fl_value; i++) {
		if(!strcmp(local_flags[i].fl_name, "echok"))
			ttyp.c_lflag^=local_flags[i].fl_value;
	}

	tcsetattr(fd, TCSANOW, &ttyp);
}

/*Toggling*/
void show_some_flags(struct termios *ttyp) {
	show_flagset(ttyp->c_iflag, input_flags);
	printf("\n \n");
	show_flagset(ttyp->c_oflag, output_flags);
	printf("\n \n");
	show_flagset(ttyp->c_lflag, local_flags);
	printf("\n \n");
}

void show_flagset(int thevalue, struct flaginfo thebitnames[]) {
	int i;
	for (i=0; thebitnames[i].fl_value; i++) {
		if(thevalue & thebitnames[i].fl_value) { //특정 플래그가 켜져 있는가?
			printf("%s ", thebitnames[i].fl_name);
		}
		else {
			printf("-%s ", thebitnames[i].fl_name);
		}
	}
}
