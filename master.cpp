#include "master.h"

Master::Master(const char* ip, int port1, int port2, const char* client_ip, int client_port, unsigned interval = 60){
    _ip = new char[20];
    _client_ip = new char[20];

    strcpy(_ip, ip);
    strcpy(_client_ip, client_ip);
    _port_to_client = port1;
    _port_to_cache = port2;
    _client_port = client_port;
    _interval = interval;
}

Master::~Master(){
    delete _ip;
    delete _client_ip;
}

void Master::run_master(){
    std::thread _thread_listen_to_client(listen_to_client, this);
    std::thread _thread_listen_heart(listen_heart, this);
    std::thread _thread_check_heart(check_cache, this);
    _thread_listen_to_client.join();
    _thread_listen_heart.join();
    _thread_check_heart.join();
}

// 线程1，接收 client 的查询分布请求
void listen_to_client(Master* master){
    SocketServer server_to_client(master->_ip, master->_port_to_client);
    while(true){
        char* recvline = server_to_client.listen_without_close();
        master->_hash.put(recvline);
        // TODO: sendline
        char* sendline = recvline;
        server_to_client.response_and_close(sendline);
    }
}

// 线程2，接收 cache 的心跳信息
void listen_heart(Master* master){
    SocketServer server_heart(master->_ip, master->_port_to_cache);
    while(1){
        char* recvline = server_heart.listen_once();
        string ip = recvline;
        if (master->_last_time.count(ip) == 0){
            master->_hash.add_real_node(ip, 300);
        }
        master->_last_time[recvline] = time(NULL);
    }
}

// 线程3，检查心跳信息，完成容灾
void check_cache(Master* master){
    std::string bad_cache;
    while(true){
        master->check_heartpoint();
        std::this_thread::sleep_for(std::chrono::seconds(master->_interval));
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
        _client_to_client.send_line(_client_ip, _client_port, "clear");
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
