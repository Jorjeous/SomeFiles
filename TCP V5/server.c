#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "string.h"


#define MAX_NICK 31
#define MAX_MSG 101
#define MAX_BUFF 201


void str_trim_lf (char* str, int length);
void str_overwrite_stdout();


// Global
volatile sig_atomic_t flag = 0;//не пытаемся преобразовать
int sockfd = 0;
char nickname[MAX_NICK] = {};

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}

void recv_msg_handler() {
    char receiveMessage[MAX_BUFF] = {};
    while (1) {
        int receive = recv(sockfd, receiveMessage, MAX_BUFF, 0);
        if (receive > 0) {
            printf("\r%s\n", receiveMessage);
            str_overwrite_stdout();
        } else if (receive == 0) {
            break;
        } else {
            // -1
        }
    }
}

void send_msg_handler() {
    char message[MAX_MSG] = {};
    while (1) {
        str_overwrite_stdout();
        while (fgets(message, MAX_MSG, stdin) != NULL) {
            str_trim_lf(message, MAX_MSG);
            if (strlen(message) == 0) {
                str_overwrite_stdout();
            } else {
                break;
            }
        }
        send(sockfd, message, MAX_MSG, 0);
        if (strcmp(message, "exit") == 0) {
            break;
        }
    }
    catch_ctrl_c_and_exit(2);
}

int main()
{
    signal(SIGINT, catch_ctrl_c_and_exit);

    printf("Please enter your name: ");
    if (fgets(nickname, MAX_NICK, stdin) != NULL) {
        str_trim_lf(nickname, MAX_NICK);
    }
    if (strlen(nickname) < 2 || strlen(nickname) >= MAX_NICK-1) {
        printf("\nName must be from 2 to 30 symbols.\n");
        exit(1);
    }

    // Create socket
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1) {
        printf("Fail to create a socket.");
        exit(2);
    }

    // Socket info
    struct sockaddr_in server_info, client_info;
    int s_addrlen = sizeof(server_info);
    int c_addrlen = sizeof(client_info);
    memset(&server_info, 0, s_addrlen);
    memset(&client_info, 0, c_addrlen);
    server_info.sin_family = PF_INET;
    server_info.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_info.sin_port = htons(8888);

    // соединяемся с сервером
    int err = connect(sockfd, (struct sockaddr *)&server_info, s_addrlen);
    if (err == -1) {
        printf("Connection to Server error!\n");
        exit(3);
    }

    // Name
    getsockname(sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen); //имя текущего сокета
    getpeername(sockfd, (struct sockaddr*) &server_info, (socklen_t*) &s_addrlen); //имя машины, подкл к серверу
    printf("Connect to Server: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));
    printf("You are: %s:%d\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

    send(sockfd, nickname, MAX_NICK, 0);

    pthread_t send_msg_thread;
    if (pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(4);
    }

    pthread_t recv_msg_thread;
    if (pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(5);
    }

    while (1) {
        if(flag) {
            printf("\nBye\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}

void str_trim_lf (char* str, int length) {
    int i;
    for (i = 0; i < length; i++) { // нарезаем по \n
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
}

void str_overwrite_stdout() {
    printf("\r%s", "> ");
    fflush(stdout); //очищаем, на всякий
}
