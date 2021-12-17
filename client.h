#include <cstring>
#include <map>
#include <random>
#include <string>  
#include <thread>
#include "socket_client.h"
#include "socket_server.h"
#include "Utils.h"

std::string strRand(int length = 8);
int keyRand();
std::pair<std::string, int>  gendata();

class Client{
public:
    Client(const char* ip, int port, const char* master_ip, int master_port, time_t _interval);
    ~Client();
    void run_client();
    void listen_to_master();
    std::pair<std::string, int> request_master(std::string sendline);
    bool write_local(std::string key, std::pair<std::string, int> cache);
    void clear_local();
    char* request_cache(const char* ip, int port, std::string data);
private:
    char* _ip;
    int _port;
    char* _master_ip;
    int _master_port;
    SocketClient _socket_client;
    
    const static int MAXLEN = 4096;
    char* buff;

    std::map<std::string, std::pair<std::string, int> > _local_hash;
    bool _is_write = true;
    time_t _interval;
};