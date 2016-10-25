//
//  main.cpp
//  chatclient
//
//  Created by yamatonao on 2016/10/25.
//  Copyright © 2016年 yamatonao. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <pthread.h>

#define PORT 12345

void* readthreadfunction(void *p);

int main(int argc,char *argv[]){
    char ipaddress[16];
    struct sockaddr_in server;
    strcpy(ipaddress, "127.0.0.1");
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    memset((char *)&server, 0, sizeof(server));

    server.sin_family       = AF_INET;
    server.sin_port         = htons(PORT);
    server.sin_addr.s_addr  = inet_addr(ipaddress);
    connect(sock, (struct sockaddr *)&server, sizeof(server));
    
    pthread_t readthread;
    pthread_create(&readthread, NULL, &readthreadfunction , &sock);
    
    printf(" server connected ! \n");
    
    while(1){
        char message[80];
        printf("> ");
        fgets(message,80,stdin);
        send(sock,message,strlen(message),0);
        
        if(strncmp(message, "exit", 4) == 0){break;}
    }
    
    pthread_cancel(readthread);
    close(sock);
    
    return 0;
}

void* readthreadfunction(void *p){
    int sock = *((int *)p);
    while(1){
        char buf[BUFSIZ];
        memset(buf, '\0', sizeof(buf));
        read( sock, buf, sizeof(buf));
        printf("from server > %s",buf);
    }
    //never reach
    //pthread_exit(NULL);
}
