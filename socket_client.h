#include <iostream>
#include<cstring>
#include<cerrno>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

class SocketClient{
public:
    char* send_line(std::string ip_port, const char* sendline);
    char* send_line(const char* ip, int port, const char* sendline);
private:
    bool _connect_and_send(const char* ip, int port, const char* sendline);
    bool _send_stdin(const char* ip, int port);
    char* _send_to_port(const char* ip, int port, const char* sendline);

    const static int MAXSIZE = 4096;
    char sendline[MAXSIZE], recvline[MAXSIZE];
    int sockfd, n;
    struct sockaddr_in  servaddr;
};


