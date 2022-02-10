#include <stdio.h>
#include <json-c/json.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "../inc/parse_json.h"
#include <arpa/inet.h>

#define TIME_VALUE_SEC 10
#define SIZE 1024

void ParseJson() {
	FILE*fpnew=NULL;
	fpnew=fopen("/tmp/CurrStats.txt","w");
	struct json_object *acc_obj, *acc_array, *acc_array_obj, *acc_array_obj_ssid, *acc_array_obj_snr, *acc_array_obj_channel;
	int arraylen, i;
	char name[10] = {0};
	const char filename[] = "/tmp/access_points";
	acc_obj = json_object_from_file(filename);
	acc_array = json_object_object_get(acc_obj, "access_points");
	arraylen = json_object_array_length(acc_array);
	for (i = 0; i < arraylen; i++) {
		acc_array_obj = json_object_array_get_idx(acc_array, i);
		acc_array_obj_ssid = json_object_object_get(acc_array_obj, "ssid");
		acc_array_obj_snr = json_object_object_get(acc_array_obj, "snr");
		acc_array_obj_channel = json_object_object_get(acc_array_obj, "channel");
		if(fpnew) {
			fprintf(fpnew, "ssid=%s\n", json_object_get_string(acc_array_obj_ssid));
			fprintf(fpnew, "snr=%s\n", json_object_get_string(acc_array_obj_snr));
			fprintf(fpnew, "channel=%s\n", json_object_get_string(acc_array_obj_channel));
		}
	}
	fclose(fpnew);
}

void *threadproc(void *arg) {
	int Start=1;
	int sockfd = *((int *) arg);
	free(arg);
	while(1) {
		ParseJson();
		char command[50];
		if(Start) {
			strcpy( command, "cp /tmp/CurrStats.txt /tmp/OldStats.txt" );
			system(command);
			Start=0;
		} else {
			CheckDeletedElement(sockfd);
			CheckAddedElement(sockfd);
			CheckModifiedElement(sockfd);
			strcpy( command, "cp /tmp/CurrStats.txt /tmp/OldStats.txt" );
			system(command);
		}
		sleep(TIME_VALUE_SEC);
	}
	return 0;
}

char* GetParam(char buffer[BUF_MAX_SIZE]) {
	char* Ret=malloc(50*sizeof(char));
	for(int i=0;i<strlen(buffer);i++) {
		if(buffer[i]=='=') {
			strncpy(Ret,buffer+i+1,strlen(buffer)-i);
		}
	}
	Ret[strlen(Ret)-1]='\0';
	return Ret;
}

void CheckDeletedElement(int sockfd) {
	FILE*fpnew=NULL;
	FILE*fpold=NULL;
	char lnew[BUF_MAX_SIZE];
	char lold[BUF_MAX_SIZE];
	int isfound=0;
	if((fpold=fopen("/tmp/OldStats.txt","r")) == NULL) {
		printf("Failed to open OldStats.txt\n");
	}
	while(fgets(lold, 512, fpold) != NULL) {
		isfound=0;
		if((fpnew=fopen("/tmp/CurrStats.txt","r")) == NULL) {
			printf("Failed to open CurrStats.txt\n");
		}
		while(fgets(lnew,512,fpnew) != NULL) {
			if(!strcmp(lnew,lold)) {
				isfound=1;
				for(int i=0;i<2;i++) {
					fgets(lold,512,fpold);
				}
				break;
			}
		}
		if(!isfound) {
			char data[SIZE];
			char* ssid=GetParam(lold);
			fgets(lold, 512, fpold);
			fgets(lold, 512, fpold);
			sprintf(data,"[+]%s is removed from the list", ssid);
			if(send(sockfd, data, sizeof(data), 0) == -1) {
				perror("[-]Error in sending data.");
				exit(1);
			}else {
				printf("[+]Data sent successfully.\n");
			}
			bzero(data, SIZE);
			free(ssid);
		}
	}
	fclose(fpold);
	fclose(fpnew);
}

void CheckAddedElement(int sockfd) {
	FILE*fpnew=NULL;
	FILE*fpold=NULL;
	char lnew[BUF_MAX_SIZE];
	char lold[BUF_MAX_SIZE];
	int isfound=0;
	if((fpnew=fopen("/tmp/CurrStats.txt","r")) == NULL) {
		printf("Failed to open CurrStats.txt\n");
	}
	while(fgets(lnew, 512, fpnew) != NULL) {
		isfound=0;
		if((fpold=fopen("/tmp/OldStats.txt","r")) == NULL) {
			printf("Failed to open OldStats.txt\n");
		}
		while(fgets(lold,512,fpold) != NULL) {
			if(!strcmp(lnew,lold)) {
				isfound=1;
				for(int i=0;i<2;i++) {
					fgets(lnew, 512, fpnew);
				}
				break;
			}
		}
		if(!isfound) {
			char data[SIZE];
			char* ssid=GetParam(lnew);
			fgets(lnew, 512, fpnew);
			char* snr=GetParam(lnew);
			fgets(lnew, 512, fpnew);
			char* channel=GetParam(lnew);
			sprintf(data,"[+]%s is added to the list with SNR %s and channel %s", ssid, snr, channel);
			if(send(sockfd, data, sizeof(data), 0) == -1) {
				perror("[-]Error in sending data.");
				exit(1);
			}else {
				printf("[+]Data sent successfully.\n");
			}
			bzero(data, SIZE);
			free(ssid);
			free(snr);
			free(channel);
		}
	}
	fclose(fpold);
	fclose(fpnew);
}

void CheckModifiedElement(int sockfd) {
	FILE*fpnew=NULL;
	FILE*fpold=NULL;
	char lnew[BUF_MAX_SIZE];
	char lold[BUF_MAX_SIZE];
	if((fpnew=fopen("/tmp/CurrStats.txt","r")) == NULL) {
		printf("Failed to open CurrStats.txt\n");
	}
	while(fgets(lnew, 512, fpnew) != NULL) {
		if((fpold=fopen("/tmp/OldStats.txt","r")) == NULL) {
			printf("Failed to open OldStats.txt\n");
		}
		while(fgets(lold,512,fpold) != NULL) {
			if((!strcmp(lnew,lold)) && (strstr(lnew,"ssid") != NULL)) {
				char data[SIZE];
				char* ssid=GetParam(lnew);
				for(int i=0;i<2;i++) {
					fgets(lnew, 512, fpnew);
					fgets(lold, 512, fpold);
					if(strcmp(lnew,lold) && (i==0)) {
						char* OldSnr=GetParam(lold);
						char* CurrSnr=GetParam(lnew);
						sprintf(data,"[+]%s’s SNR has changed from %s to %s", ssid, OldSnr, CurrSnr);
						if(send(sockfd, data, sizeof(data), 0) == -1) {
							perror("[-]Error in sending data.");
							exit(1);
						}else {
							printf("[+]Data sent successfully.\n");
						}
						bzero(data, SIZE);
						free(OldSnr);
						free(CurrSnr);
					}else if(strcmp(lnew,lold) && (i==1)) {
						char* OldChannel=GetParam(lold);
						char* CurrChannel=GetParam(lnew);
						sprintf(data,"[+]%s’s channel has changed from %s to %s", ssid, OldChannel, CurrChannel);
						if(send(sockfd, data, sizeof(data), 0) == -1) {
							perror("[-]Error in sending data.");
							exit(1);
						}else {
							printf("[+]Data sent successfully.\n");
						}
						bzero(data, SIZE);
						free(OldChannel);
						free(CurrChannel);
					}
				}
				free(ssid);
			}
		}
	}
	fclose(fpold);
	fclose(fpnew);
}

int main(int argc, char **argv) {

	char *ip = "127.0.0.1";
	int port = 8080;
	int e;
	int sockfd;
	struct sockaddr_in server_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("[-]Error in socket");
		exit(1);
	}
	printf("[+]Client socket created successfully.\n");
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(e == -1) {
		perror("[-]Error in socket");
		exit(1);
	}
	int *arg = malloc(sizeof(*arg));
	*arg=sockfd;
	pthread_t tid;
	pthread_create(&tid, NULL, &threadproc, arg);
	pthread_join(tid, NULL);
	return 0;
}
