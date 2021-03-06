#ifndef __MASTER_H__
#define __MASTER_H__

#include <ctime>
#include <mutex>
#include <vector>
#include <string>
#include <thread>
#include "socket_client.h"
#include "socket_server.h"
#include "my_hash.h"

class Master;
void listen_to_client(Master* master);
void listen_heart(Master* master);
void check_cache(Master* master);

class Master{
public:
    Master(const char* ip, int port1, int port2, const char* client_ip, int client_port, size_t cache_size, time_t interval);
    ~Master();
    void run_master();
    friend void listen_to_client(Master* master);
    friend void listen_heart(Master* master);
    friend void check_cache(Master* master);
    
    void check_heartpoint();
    void remove_cache(std::string bad_cache);
    void reset_cache();
    void add_cache(std::string cache_ip);

private:
    char* _ip;
    int _port_to_client;
    int _port_to_cache;
    
    char* _client_ip;
    int _client_port;
    
    SocketClient _client_to_cache;
    SocketClient _client_to_client;

    size_t _cache_size;
    time_t _interval;
    std::mutex _lock_hash;
    std::mutex _lock_heart;
    consistent_hash _hash;
    std::map<std::string, time_t> _last_time;
};

#endif 
