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
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n", ip);
        return false;
    }
    servaddr.sin_port = htons(port);
    
    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }
    return true;
}

char* SocketServer::listen_once(){
    int n = 0;
    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return nullptr;
    }
    if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        return nullptr;
    }
    n = recv(connfd, buff, MAXSIZE, 0);
    while (buff[n - 1] == '\r' || buff[n - 1] == '\n'){
        n--;
    }
    // std::cout << n << std::endl;
    buff[n] = '\0';
    printf("recv msg from client: %s\n", buff);
    close(connfd);
    close(listenfd);
    return buff;
}

char* SocketServer::listen_without_close(){
    int n = 0;
    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return nullptr;
    }
    if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        return nullptr;
    }
    n = recv(connfd, buff, MAXSIZE, 0);
    while (buff[n - 1] == '\r' || buff[n - 1] == '\n'){
        n--;
    }
    buff[n] = '\0';
    return buff;
}

bool SocketServer::response_and_close(char* sendline){
    printf("send msg to client: %s\n", sendline);
    if( send(connfd, sendline, strlen(sendline), 0) < 0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return false;
    }
    close(connfd);
    close(listenfd);
    return true;
}

bool SocketServer::listen_to_port(char* (*function)(char*)){
    int n = 0;
    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    printf("======waiting for client's request======\n");
    while(1){
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        n = recv(connfd, buff, MAXSIZE, 0);
        while (buff[n - 1] == '\r' || buff[n - 1] == '\n'){
            n--;
        }
        std::cout << n << std::endl;
        buff[n] = '\0';
        printf("recv msg from client: %s\n", buff);

        auto sendline = function(buff);
        printf("send msg to client: %s\n", sendline);
        if( send(connfd, sendline, strlen(sendline), 0) < 0){
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            return 0;
        }
        close(connfd);
    }
    close(listenfd);
    return 0;
}

char* fun(char* a){
    char* b = new char[4096];
    int len = strlen(a);
    strcpy(b, a);
    strcpy(b + len, a);
    return b;
}
