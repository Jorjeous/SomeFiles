#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<pthread.h>

//#define PORT 44212
#define MAX_USERS 9

char buffer[1024];

void* RecvAndSend(struct ClientData* Data);



struct ClientData
{
    int ClientSockets[MAX_USERS];
    char ClientName[MAX_USERS][50];
    struct sockaddr_in newAddr;h    struct sockaddr_in serverAddr;
    socklen_t addr_size;
}Data ;



int main(int argc, char* argv[]){


        pthread_t thread;

        int status;
        int PORT = atoi(argv[1]);
        int sockfd, ret, count = 0;


         for(int i=0;i<MAX_USERS;i++){
             strcpy(Data.ClientName[i],"");
             Data.ClientSockets[i] = 0;
         }

	int newSocket;
        //struct sockaddr_in newAddr;




	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

        memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        ret = bind(sockfd, (struct sockaddr*)&Data.serverAddr, sizeof(Data.serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
        printf("[+]Bind to port %d\n", PORT);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}
        bzero(buffer, sizeof(buffer));


	while(1){
                newSocket = accept(sockfd, (struct sockaddr*)&Data.newAddr, &Data.addr_size);
		if(newSocket < 0){
			exit(1);
                        printf("point 133");
		}
                printf("Connection accepted from %s:%d\n", inet_ntoa(Data.newAddr.sin_addr), ntohs(Data.newAddr.sin_port));
                status = pthread_create(&thread, NULL, RecvAndSend, &Data);
                if(status != 0)
                {
                    printf("ErrorPthread");
                    exit(1);
                }





	}

	close(newSocket);


	return 0;
}



void* RecvAndSend(struct ClientData* Data){
    int newSocket;
    while(1){

            recv(newSocket, buffer, 1024, 0);
            for(int i = 0; i < MAX_USERS; i++ ){
                if(Data->ClientSockets[i] == newSocket )
                {
                    break;
                }

                if(Data->ClientSockets[i] != 0 ){
                    continue;
                }
                if(Data->ClientSockets[i] == 0){
                    Data->ClientSockets[i] = newSocket;
                    printf("SOCK{%d}:%d\n",i, Data->ClientSockets[i]);
                    strcpy(Data->ClientName[i], buffer);
                    printf("Name{%d}:%s\n",i, Data->ClientName[i] );
                    break;
                }
            }
            if(buffer[0]==':') {
                printf("first is :\n");
                if(buffer[1] == 'e'){
                    for(int i = 0; i < MAX_USERS; i++){
                        if(Data->ClientSockets[i] == newSocket){
                            strcpy(Data->ClientName[i], "");
                            Data->ClientSockets[i] = 0;
                        }
                    }
                    printf("Disconnected from %s:%d\n", inet_ntoa(Data->newAddr.sin_addr), ntohs(Data->ClientNamenewAddr.sin_port));
                    break;
                }

           }else{
                printf("TRYING TO SENT TO ALL\n");
                for(int i = 0; i < MAX_USERS; i++){
                    send(Data->ClientSockets[i], buffer, strlen(buffer), 0);
                    printf("sended to %s\n", Data->ClientName[i]);
                }
                bzero(buffer, sizeof(buffer));
            }

            /*else{
                    printf("Client: %s\n", buffer);
                    printf("Going to send to %d",newSocket);
                    send(newSocket, buffer, strlen(buffer), 0);
                    bzero(buffer, sizeof(buffer));
            }*/
    }
}



//--------------
/*if((childpid = fork()) == 0){
        close(sockfd);


}*/
/*while(count < MAX_USERS){
    Data.ClientSockets[count] = newSocket;
    strcpy(Data.ClientName[count], buffer);
    printf("SOCK{%d}:%d\n",count, Data.ClientSockets[count]);
    printf("Name{%d}:%s\n",count, Data.ClientName[count] );
    count=count+1;

}*/
//CollectInfo(newAddr.sin_addr, newAddr.sin_port);


