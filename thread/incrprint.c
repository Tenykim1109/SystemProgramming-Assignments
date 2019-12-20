#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM 5

int counter = 0;

void main() {
	pthread_t t1;
	void *print_count(void*);
	int i;

	pthread_create(&t1, NULL, print_count, NULL);
	for(i=0; i<NUM; i++) {
		counter++;
		sleep(1);
	}

	pthread_join(t1, NULL);
	return;
}

void *print_count(void* m) {
	int i;
	for(i=0; i<NUM; i++) {
		printf("count = %d\n", counter);
		sleep(1);
	}
	return NULL;
}


