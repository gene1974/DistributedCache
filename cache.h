#ifndef __CACHE_H__
#define __CACHE_H__

#include <mutex>
#include <thread>
#include <sstream>
#include <string>
#include "LRUCache.h"
#include "socket_client.h"
#include "socket_server.h"

class Cache;
void listen_to_client(Cache* cache);
void listen_to_master(Cache* cache);
void heart(Cache* cache);

class Cache{
public:
    Cache(const char* ip, int port1, int port2, const char* master_ip, int master_port, int size, int interval);
    ~Cache();
    void run_cache();
    friend void listen_to_client(Cache* cache);
    friend void listen_to_master(Cache* cache);
    friend void heart(Cache* cache);

    int query_cache(char* line);
    void reset_cache(char* recvline);

private:
    char* _ip;
    int _port_to_client;
    int _port_to_master;
    char* _ip_port; // "ip:port"
    
    char* _master_ip;
    int _master_port;
    SocketClient _client_to_master;
    
    const static int MAXLEN = 256;
    char* _buff;

    int _size;
    int _interval;
    std::mutex _cache_lock;
    LRU_Cache* _lruCache;
};

#endif 
