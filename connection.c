#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include "connection.h"

/*Server*/
int getServerSock(short port){
    int rc, tmp;
    int listenfd;
    struct sockaddr_in servaddr;
    int yes = 1;

    //ソケット生成
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0){
        printf("create sock fail\n");
        return -1;
    }

    //ソケット設定
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

    //アドレス生成
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    //ポート接続
    rc = bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(rc < 0){
        printf("bind fail\n");
        return -1;
    }

    //接続待ち
    rc = listen(listenfd, 5);
    if(rc < 0){
        printf("listen fail\n");
        return -1;
    }
    return listenfd;
}

int acceptSock(int listenfd){
    struct sockaddr_in cliaddr;
    int rc, tmp, clientfd;
    //クライアントソケット生成
    tmp = sizeof(cliaddr);
    clientfd = accept(listenfd, (struct sockaddr*)&cliaddr, (socklen_t*)&tmp);
    if(clientfd < 0){
        printf("accept fail\n");
        return -1;
    }
    printf("accept ip %d\naccept port %d\n", cliaddr.sin_addr.s_addr, cliaddr.sin_port);
    return clientfd;
}

/*client*/
int getClientSock(char* ip, short port){
    int rc;
    int sockfd;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    
    //ソケット生成
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("create sock fail\n");
        return -1;
    }

    //アドレス生成
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    //接続
    rc = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(rc < 0){
        printf("connect fail\n");
        return -1;
    }

    return sockfd;

}

void sigpipeHandler(int sig){
    printf("SIGPIPE\n");
    return;
}
