#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <cstring>
#include <map>
#include <mutex>
#include <random>
#include <string>  
#include <thread>
#include "socket_client.h"
#include "socket_server.h"
#include "Utils.h"

class Client;
std::string strRand(int length);
int keyRand();
std::pair<std::string, int>  gendata();
void listen_to_master(Client* client);

class Client{
public:
    Client(const char* ip, int port, const char* master_ip, int master_port, time_t _interval);
    ~Client();
    void run_client();
    friend void listen_to_master(Client* client);

    
    std::string get_distribution(std::string key);
    std::string request_master(std::string sendline);
    void clear_local();
    char* request_cache(const char* ip, int port, std::string data);
    
private:
    char* _ip;
    int _port_to_master;
    
    char* _master_ip;
    int _master_port;
    SocketClient _socket_client;
    
    const static int MAXLEN = 4096;
    char* buff;

    std::map<std::string, std::string> _local_hash;
    std::mutex _hash_lock;
    bool _is_write = true;
    time_t _interval;
};

#endif 
