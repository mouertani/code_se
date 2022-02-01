#include <stdio.h>
#include <json-c/json.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "../inc/parse_json.h"

#define TIME_VALUE_SEC 10

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
	while(1) {
		ParseJson();
		char command[50];
		if(Start) {
			strcpy( command, "cp /tmp/CurrStats.txt /tmp/OldStats.txt" );
			system(command);
			Start=0;
		} else {
			CheckDeletedElement();
			CheckAddedElement();
			CheckModifiedElement();
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

void CheckDeletedElement() {
	FILE*fpnew=NULL;
	FILE*fpold=NULL;
	FILE*fpout=NULL;
	char lnew[BUF_MAX_SIZE];
	char lold[BUF_MAX_SIZE];
	int isfound=0;
	if((fpout=fopen("/tmp/Results.txt","w")) == NULL) {
		printf("Failed to open Results.txt\n");
	}
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
			char* ssid=GetParam(lold);
			fgets(lold, 512, fpold);
			fgets(lold, 512, fpold);
			fprintf(fpout,"%s is removed from the list\n", ssid);
			free(ssid);
		}
	}
	fclose(fpout);
	fclose(fpold);
	fclose(fpnew);
}

void CheckAddedElement() {
	FILE*fpnew=NULL;
	FILE*fpold=NULL;
	FILE*fpout=NULL;
	char lnew[BUF_MAX_SIZE];
	char lold[BUF_MAX_SIZE];
	if((fpout=fopen("/tmp/Results.txt","a")) == NULL) {
		printf("Failed to open Results.txt\n");
	}
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
			char* ssid=GetParam(lnew);
			fgets(lnew, 512, fpnew);
			char* snr=GetParam(lnew);
			fgets(lnew, 512, fpnew);
			char* channel=GetParam(lnew);
			fprintf(fpout,"%s is added to the list with SNR %s and channel %s\n", ssid, snr, channel);
			free(ssid);
			free(snr);
			free(channel);
		}
	}
	fclose(fpout);
	fclose(fpold);
	fclose(fpnew);
}

void CheckModifiedElement() {
	FILE*fpnew=NULL;
	FILE*fpold=NULL;
	FILE*fpout=NULL;
	char lnew[BUF_MAX_SIZE];
	char lold[BUF_MAX_SIZE];
	if((fpout=fopen("/tmp/Results.txt","a")) == NULL) {
		printf("Failed to open Results.txt\n");
	}
	if((fpnew=fopen("/tmp/CurrStats.txt","r")) == NULL) {
		printf("Failed to open CurrStats.txt\n");
	}
	while(fgets(lnew, 512, fpnew) != NULL) {
		if((fpold=fopen("/tmp/OldStats.txt","r")) == NULL) {
			printf("Failed to open OldStats.txt\n");
		}
		while(fgets(lold,512,fpold) != NULL) {
			if(!strcmp(lnew,lold)) {
				char* ssid=GetParam(lnew);
				for(int i=0;i<2;i++) {
					fgets(lnew, 512, fpnew);
					fgets(lold, 512, fpold);
					if(strcmp(lnew,lold) && (i==0)) {
						char* OldSnr=GetParam(lold);
						char* CurrSnr=GetParam(lnew);
						fprintf(fpout,"%s’s SNR has changed from %s to %s\n", ssid, OldSnr, CurrSnr);
						free(OldSnr);
						free(CurrSnr);
					}else if(strcmp(lnew,lold) && (i==1)) {
						char* OldChannel=GetParam(lold);
						char* CurrChannel=GetParam(lnew);
						fprintf(fpout,"%s’s channel has changed from %s to %s\n", ssid, OldChannel, CurrChannel);
						free(OldChannel);
						free(CurrChannel);
					}
				}
				free(ssid);
			}
		}
	}
	fclose(fpout);
	fclose(fpold);
	fclose(fpnew);
}

int main(int argc, char **argv) {
	pthread_t tid;
	pthread_create(&tid, NULL, &threadproc, NULL);
	pthread_join(tid, NULL);
	return 0;
}
