#include<stdio.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <unistd.h> 
#include<string.h>
#include<pthread.h>
#define SA struct sockaddr
#define BUFF_SIZE 100

char username[20];
char msg[BUFF_SIZE];

void* write_serv(void *arg){
	int sock = *((int*)arg);
	char MSG[20+BUFF_SIZE];
	while(1){
		fgets(msg,BUFF_SIZE,stdin);
		if(!strcmp(msg,"q\n") || !strcmp(msg,"Q\n")){
			close(sock);
			exit(0);
		}
		sprintf(MSG,"%s %s",username,msg);
		write(sock,MSG,strlen(MSG));
	}
	return NULL;
}

void* read_serv(void *arg){
	int sock = *((int*)arg);
	char MSG[20+BUFF_SIZE];
	int str_len;
	while(1){
		str_len = read(sock,MSG,20+BUFF_SIZE-1);
		if(str_len == -1){
			return (void*)-1;
		}
		MSG[str_len] = 0;
		fputs(MSG,stdout);
	}
	return NULL;
}


int main(int argc,char *argv[]){
	struct sockaddr_in selfAddr;
	int sockFd;
	pthread_t snd,rcv;
	void *thread_ret;
	
	if(argc < 4){
		printf("Enter './exefile serv_IP port user_name'\n");
		exit(0);
	}
	
	sprintf(username,"[ %s ] ",argv[3]);
	
	sockFd = socket(AF_INET,SOCK_STREAM,0);
	if(sockFd==-1){
		printf("Failed to create socket\n");
		exit(0);
	}
	printf("Socket Created\n");
	
	bzero(&selfAddr, sizeof(selfAddr));
	
	selfAddr.sin_family = AF_INET;
	selfAddr.sin_addr.s_addr  = inet_addr(argv[1]);
	selfAddr.sin_port = htons(atoi(argv[2]));
	
	if(connect(sockFd,(struct sockaddr*)&selfAddr,sizeof(selfAddr))!=0){
		printf("Failed to connect\n");
		exit(0);
	}
	printf("Connected\n");
	
	pthread_create(&snd,NULL,write_serv,(void*)&sockFd);
	pthread_create(&rcv,NULL,read_serv,(void*)&sockFd);
	pthread_join(snd,&thread_ret);
	pthread_join(rcv,&thread_ret);
	close(sockFd);
	return 0;
}