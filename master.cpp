#include "master.h"

Master::Master(const char* ip, int port1, int port2, const char* client_ip, int client_port, size_t cache_size = 60, time_t interval = 60){
    _ip = new char[20];
    _client_ip = new char[20];

    strcpy(_ip, ip);
    strcpy(_client_ip, client_ip);
    _port_to_client = port1;
    _port_to_cache = port2;
    _client_port = client_port;
    _cache_size = cache_size;
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
    char* recvline = nullptr;
    std::string sendline;
    while(true){
        recvline = server_to_client.listen_without_close();
        master->_lock_hash.lock();
        sendline = master->_hash.put(recvline);
        master->_lock_hash.unlock();
        server_to_client.response_and_close(sendline.c_str());
    }
}

// 线程2，接收 cache 的心跳信息
void listen_heart(Master* master){
    SocketServer server_heart(master->_ip, master->_port_to_cache);
    char* recvline = nullptr;
    std::string ip;
    while(1){
        recvline = server_heart.listen_once();
        ip = recvline; // "ip:port"
        
        master->_lock_hash.lock();
        if (master->_last_time.count(ip) == 0){
            master->_hash.add_real_node(ip, 300);
        }
        master->_lock_hash.unlock();

        master->_lock_heart.lock();
        master->_last_time[recvline] = time(NULL);
        master->_lock_heart.unlock();
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
    std::string bad_cache;
    time_t now = time(NULL);
    time_t heart_time = 0;

    _lock_heart.lock();
    for (auto iter = _last_time.begin(); iter != _last_time.end(); iter++){
        heart_time = iter->second;
        if (now - heart_time > _interval){
            bad_cache = iter->first;
            need_reset = true;
            remove_cache(bad_cache);
            _last_time.erase(bad_cache);
            iter = _last_time.begin(); // 有删除则重新遍历
        }
    }
    _lock_heart.unlock();

    if (need_reset){
        reset_cache();
        _client_to_client.send_line(_client_ip, _client_port, "clear");
    }
}

void Master::remove_cache(std::string bad_cache){
    _lock_hash.lock();
    _hash.drop_real_node(bad_cache);
    _lock_hash.unlock();
}

void Master::reset_cache(){
    char sendline[255];
    int cache_num = 0;
    int new_cache_size = 0;

    _lock_hash.lock();
    cache_num =  _hash.get_node_num();
    if (cache_num == 0){
        std::cout << "All caches are broken!" << std::endl;
        _lock_hash.unlock();
        return;
    }
    new_cache_size = (int)(_cache_size / cache_num);
    sprintf(sendline, "%d", new_cache_size);
    for(auto iter = _hash.real_node_map.begin(); iter != _hash.real_node_map.end(); iter++){
        _client_to_cache.send_line(iter->first, sendline);
    }
    _lock_hash.unlock();
}

void Master::add_cache(std::string cache_ip){
    _lock_hash.lock();
    _hash.add_real_node(cache_ip, 300);
    _hash.get_node_num();
    _lock_hash.unlock();
}
