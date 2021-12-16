
#include <string>  
#include "LRUCache.h"
#include "socket_client.h"
#include "socket_server.h"

class Cache{
public:
    Cache(const char* ip, int port1, int port2, const char* master_ip, int master_port, int size);
    ~Cache();
    void run_cache();
    void write_cache(char* line);
    void reset_cache(char* recvline);
    void listen_to_client();
    void listen_to_master();

    void heart();

    std::pair<char*, int> request_master(std::string sendline);
    bool write_local(std::string key, std::pair<char*, int> cache);
    char* request_cache(const char* ip, int port, std::string data);
    int get_size();
    void set_size();
private:
    char* _ip;
    int _port_to_client;
    int _port_to_master;
    
    char* _master_ip;
    int _master_port;
    SocketClient _client_to_master;
    
    const static int MAXLEN = 4096;
    char* _buff;

    int _size;
    LRU_Cache* _lruCache;
};