#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PATHNAME 30

void do_mv(char*, char*);
int check_dup_file(char*);

int main(int argc, char *argv[]) {
	if(argc!=3) {
		fprintf(stderr, "Usage: command <oldpath> <newpath>\n");
		exit(1);
	}
	else
		do_mv(argv[1], argv[2]);

	return 0;
}

void do_mv(char oldpath[], char newpath[]) {
	struct stat file_info;
	int result;
	char yes_or_no;
	char info_path[PATHNAME];

	if(strcmp(oldpath, newpath)==0) {
		fprintf(stderr, "Those arguments are the same name.\n");
		exit(1);
	}

	if(stat(newpath, &file_info)!=-1) {
		if(S_ISDIR(file_info.st_mode)) { //Is newpath a directory?
			strcat(newpath, "/");
			strcat(newpath, oldpath);
			strcpy(info_path, newpath);
			rename(oldpath, info_path);
		}
	}

	result=check_dup_file(newpath); //Is the file name duplicated?

	if(result==1) {
		printf("error: File Exists. \ndoing anyway? ");
		scanf("%s", &yes_or_no);
		if(!strcmp(&yes_or_no, "N") || !strcmp(&yes_or_no, "n"))
			exit(1);
		else
			rename(oldpath, newpath);
	}
	else
		rename(oldpath, newpath);
}

int check_dup_file(char newpath[]) {
	DIR *dir_ptr;
	struct dirent *namelist;
	int num;

	if((dir_ptr=opendir("."))==NULL)
		fprintf(stderr, "error!");
	else {
		while((namelist=readdir(dir_ptr))!=NULL) {
			if(strcmp(namelist->d_name, newpath)==0) //Does newpath already exists?
				return 1;
		}
	}
	return 0;
}
