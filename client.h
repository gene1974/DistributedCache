
#include <string>  
#include "SingleLinkedList.h"
#include "socket_client.h"

class Client{
public:
    Client(const char* ip, int port);
    ~Client();
    void run_client();
    std::pair<char*, int> request_master(std::string sendline);
    bool write_local(std::string key, std::pair<char*, int> cache);
    char* request_cache(const char* ip, int port, std::string data);
private:
    char* _ip;
    int _port;
    char* master_ip;
    int master_port;
    char* cache_ip;
    int cache_port;
    SocketClient socket_client;
    
    const static int MAXLEN = 4096;
    char* buff;

    SingleLinkedList<string> list;
    std::string data;
    bool is_write = true;
};