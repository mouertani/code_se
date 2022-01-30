#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <../inc/SendResult.h>

#define TIME_VALUE_SEC 10
#define BUF_MAX_SIZE 1000

void ReadData() {
	FILE*fp=NULL;
	char buffer[BUF_MAX_SIZE];
	if((fp=fopen("/tmp/Results.txt","r")) == NULL) {
		printf("Failed to open Results.txt\n");
	}
	while(fgets(buffer,512,fp) != NULL) {
		printf("%s\n", buffer);
	}
}

void *threadproc(void *arg) {
	while(1) {
		ReadData();
		sleep(TIME_VALUE_SEC);
	}
	return 0;
}

int main(int argc, char **argv) {
	pthread_t tid;
	pthread_create(&tid, NULL, &threadproc, NULL);
	pthread_join(tid, NULL);
	return 0;
}
