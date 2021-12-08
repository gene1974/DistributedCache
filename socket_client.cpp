#include "socket_client.h"

bool SocketClient::_connect(const char* ip, int port){
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return 0;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n", ip);
        return 0;
    }
    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    return true;
}

bool SocketClient::_send(const char* sendline){
    if( send(sockfd, sendline, strlen(sendline), 0) < 0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    return true;
}

bool SocketClient::_close(){
    close(sockfd);
    return true;
}

bool SocketClient::connect_server(const char* ip, int port){
    return _connect(ip, port);
}

bool SocketClient::send_line(const char* sendline){
    return _send(sendline);
}

bool SocketClient::close_connect(){
    return _close();
}

bool SocketClient::connect_and_send(const char* ip, int port, const char* sendline){
    if (!_connect(ip, port))
        return false;
    if (!_send(sendline)) 
        return false;
    if (!_close()) 
        return false;
    return true;
}

bool SocketClient::send_stdin(const char* ip, int port){
    char sendline[4096];
    while (fgets(sendline, 4096, stdin)){
        connect_and_send(ip, port, sendline);
    }
    return true;
}

// int main(int argc, char** argv){
//     char  recvline[MAXSIZE], sendline[MAXSIZE];
//     SocketClient client;
//     // while (fgets(sendline, MAXSIZE, stdin)){
//     //     client.connect_and_send("127.0.0.1", 6666, sendline);
//     // }
//     client.send_stdin("127.0.0.1", 6666);
//     return 0;
// }
