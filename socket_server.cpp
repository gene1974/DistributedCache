#include <cstring>
#include "socket_server.h"

SocketServer::SocketServer(const char* ip, int port){
    _ip = new char[20];
    strcpy(_ip, ip);
    _port = port;
    _init(ip, port);
}

SocketServer::~SocketServer(){
    delete _ip;
}

bool SocketServer::_init(const char* ip, int port){
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("[socket] create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0){
        printf("[socket] inet_pton error for %s\n", ip);
        return false;
    }
    servaddr.sin_port = htons(port);
    
    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("[socket] bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }
    return true;
}

char* SocketServer::listen_once(){
    int n = 0;
    if( listen(listenfd, 10) == -1){
        printf("[socket] listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return nullptr;
    }
    if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
        printf("[socket] accept socket error: %s(errno: %d)",strerror(errno),errno);
        return nullptr;
    }
    n = recv(connfd, buff, MAXSIZE, 0);
    while (buff[n - 1] == '\r' || buff[n - 1] == '\n'){
        n--;
    }
    buff[n] = '\0';
    printf("[server] recv msg: %s\n", buff);
    close(connfd);
    return buff;
}

char* SocketServer::listen_without_close(){
    int n = 0;
    if( listen(listenfd, 10) == -1){
        printf("[socket] listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return nullptr;
    }
    //printf("======waiting for client's request======\n");
    if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
        printf("[socket] accept socket error: %s(errno: %d)",strerror(errno),errno);
        return nullptr;
    }
    n = recv(connfd, buff, MAXSIZE, 0);
    while (buff[n - 1] == '\r' || buff[n - 1] == '\n'){
        n--;
    }
    buff[n] = '\0';
    printf("[socket] recv msg: %s\n", buff);
    return buff;
}

bool SocketServer::response_and_close(const char* sendline){
    if( send(connfd, sendline, strlen(sendline), 0) < 0){
        printf("[socket] send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return false;
    }
    printf("[socket] send msg: %s\n", sendline);
    close(connfd);
    return true;
}
