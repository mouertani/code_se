#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <../inc/SendResult.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>

#define TIME_VALUE_SEC 10
#define SIZE 10000

void *threadproc(void *arg) {
	char *ip = "127.0.0.1";
	int port = 8080;
	int e;
	int sockfd, new_sock;
	struct sockaddr_in server_addr, new_addr;
	socklen_t addr_size;
	char buffer[SIZE];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
	perror("[-]Error in socket");
		exit(1);
	}
	printf("[+]Server socket created successfully.\n");
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(e < 0) {
		perror("[-]Error in bind");
		exit(1);
	}
	printf("[+]Binding successfull.\n");
	if(listen(sockfd, 10) == 0) {
		printf("[+]Listening....\n");
	}else {
		perror("[-]Error in listening");
		exit(1);
	}
	addr_size = sizeof(new_addr);
	new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
	for(;;) {
		read(new_sock , buffer, 1024);
		printf("%s\n",buffer);
	}
	return 0;
}

int main(int argc, char **argv) {
	pthread_t tid;
	pthread_create(&tid, NULL, &threadproc, NULL);
	pthread_join(tid, NULL);
	return 0;
}
