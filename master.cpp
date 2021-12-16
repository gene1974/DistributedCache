#include <iostream>
#include <thread>

#include "socket_server.h"
#include "socket_client.h"
#include "master.h"

Master::Master(const char* ip, int port1, int port2, unsigned interval){
    _ip = new char[20];

    strcpy(_ip, ip);
    _port_to_client = port1;
    _port_to_cache = port2;
    _interval = interval;
}

Master::~Master(){
    delete _ip;
    //delete _buff;
}

void Master::run_master(){
    std::thread _thread_listen_to_client(listen_to_client);
    std::thread _thread_listen_heart(listen_heart);
    std::thread _thread_send_to_cache(send_to_cache);
    _thread_listen_to_client.join();
    _thread_listen_heart.join();
    _thread_send_to_cache.join();
}

// 线程1，接收 client 的查询分布请求
void Master::listen_to_client(){
    SocketServer server_to_client(_ip, _port_to_client);
    while(true){
        char* recvline = server_to_client.listen_without_close();
        _hash.put(recvline);
        // TODO: sendline
        char* sendline = recvline;
        server_to_client.response_and_close(sendline);
    }
}

// 线程2，接收 cache 的心跳信息
void Master::listen_heart(){
    SocketServer server_heart(_ip, _port_to_cache);
    while(1){
        char* recvline = server_heart.listen_once();
        string ip = recvline;
        if (_last_time.count(ip) == 0){
            _hash.add_real_node(ip);
        }
        _last_time[recvline] = time(NULL);
    }
}

// 线程3，整理心跳信息，完成容灾
void Master::check_cache(){
    std::string bad_cache;
    while(true){
        check_heartpoint();
        std::this_thread::sleep_for(std::chrono::seconds(_interval));
    }
}

void Master::check_heartpoint(){
    bool need_reset = false;
    time_t now = time(NULL);
    time_t heart_time = 0;
    for (auto iter = _last_time.begin(); iter != _last_time.end(); iter++){
        heart_time = iter->second;
        if (now - heart_time > _interval){
            need_reset = true;
            remove_cache(iter->first);
        }
    }
    if (need_reset){
        reset_cache();
    }
}

void Master::remove_cache(std::string bad_cache){
    _hash.drop_real_node(bad_cache);
    _last_time.erase(bad_cache);
}

void Master::reset_cache(){
    for(auto iter = _last_time.begin(); iter != _last_time.end(); iter++){
        char sendline[255];
        // TODO: number
        sprintf(sendline, "%d", 20);
        _client_to_cache.send_line(iter->first, sendline);
    }
}
