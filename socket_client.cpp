#include "socket_client.h"

bool SocketClient::connect_and_send(const char* ip, int port, const char* sendline){
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return false;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n", ip);
        return false;
    }
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }
    
    if (send(sockfd, sendline, strlen(sendline), 0) < 0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return false;
    }

    // char buff[4096];
    n = recv(sockfd, recvline, 4096, 0);
    std::cout << n << std::endl;
    recvline[n] = '\0';
    printf("recv msg from client: %s\n", recvline);

    close(sockfd);
    return true;
}

bool SocketClient::send_stdin(const char* ip, int port){
    send_to_port(ip, port, "Hi");
    while (fgets(sendline, 4096, stdin)){
        send_to_port(ip, port, sendline);
    }
    return true;
}

char* SocketClient::send_to_port(const char* ip, int port, const char* sendline){
    if (connect_and_send(ip, port, sendline)){
        return recvline;
    }
    return nullptr;
}

int main(int argc, char** argv){
    SocketClient client;
    client.send_stdin("127.0.0.1", 6666);
    return 0;
}
