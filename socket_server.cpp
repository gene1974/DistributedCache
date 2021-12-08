#include "socket_server.h"

SocketServer::SocketServer(const char* ip, int port){
    _init(ip, port);
}

bool SocketServer::_init(const char* ip, int port){
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n", ip);
        return 0;
    }
    servaddr.sin_port = htons(port);
    
    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    return true;
}

bool SocketServer::listen_to_port(){
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
        buff[n - 1] = '\0';
        printf("recv msg from client: %s\n", buff);
        close(connfd);
    }
    close(listenfd);
    return 0;
}

// int main(int argc, char** argv){
//     char recvline[MAXSIZE];
//     SocketServer socker_server("127.0.0.1", 6666);
//     socker_server.listen_to_port();
//     return 0;
// }
