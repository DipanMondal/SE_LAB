#include<stdio.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <unistd.h> 
#include<string.h>
#include<pthread.h>
#define SA struct sockaddr
#define BUFF_SIZE 100
#define MAX_CLIENT 5

int clients[MAX_CLIENT];
int curr_client = 0;
pthread_mutex_t mutex;

void send_message(char *msg,int str_len,int clntFd){
	int i;
	pthread_mutex_lock(&mutex);
	for(i=0;i<curr_client;i++)
		if(clients[i] != clntFd){
			write(clients[i],msg,str_len);
		}
	pthread_mutex_unlock(&mutex);
}

void* handle_client(void* clnt){
	int clntFd = *((int*)clnt);
	int i,str_len = 0;
	char msg[BUFF_SIZE];
	
	while((str_len = read(clntFd,msg,sizeof(msg)))!=0)
		send_message(msg,str_len,clntFd);
	
	pthread_mutex_lock(&mutex);
	
	for(i=0;i<curr_client;i++){
		if(clients[i] == clntFd){
			while(i<curr_client-1){
				clients[i] = clients[i+1];
				i++;
			}
			break;
		}
	}
	curr_client--;
	pthread_mutex_unlock(&mutex);
	close(clntFd);
	return NULL;
}

int main(int argc,char *argv[]){
	struct sockaddr_in servAddr,clntAddr;
	int sockFd,clntFd;
	int len;
	pthread_t t_id;
	char msg[] = "Sorry slot is full. Please try again later\n";
	
	if(argc < 2){
		printf("Enter './exefile port_number'\n");
		exit(0);
	}
	
	pthread_mutex_init(&mutex,NULL);
	sockFd = socket(AF_INET,SOCK_STREAM,0);
	if(sockFd==-1){
		printf("Socket Created Failed\n");
	}
	printf("Socket Created Successfully\n");
	
	bzero(&servAddr,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));
	
	if(bind(sockFd,(struct sockaddr*)&servAddr,sizeof(servAddr)) != 0){
		printf("Failed to bind\n");
		exit(0);
	}
	printf("Bind to port : %s\n",argv[1]);
	
	if(listen(sockFd,MAX_CLIENT) != 0){
		printf("Listenning failed\n");
		exit(0);
	}
	printf("Listenning...\n");
	
	
	while(1){
		
		len = sizeof(clntAddr);
		bzero(&clntAddr,sizeof(clntAddr));
		clntFd = accept(sockFd,(struct sockaddr*)&clntAddr,&len);
		printf("Total clients : %d\n",curr_client+1);
		
		pthread_mutex_lock(&mutex);
		clients[curr_client++] = clntFd;
		pthread_mutex_unlock(&mutex);
		
		pthread_create(&t_id,NULL,handle_client,(void*)&clntFd);
		pthread_detach(t_id);
		printf("Connected client IP : %s\n",inet_ntoa(clntAddr.sin_addr));
	}
	close(sockFd);
	return 0;
}
