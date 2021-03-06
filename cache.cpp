#include "cache.h"

Cache::Cache(const char* ip, int port, const char* master_ip, int master_port, int size = 5, int interval = 20){
    _ip = new char[20];
    _ip_port = new char[48];
    _master_ip = new char[20];
    _buff = new char[MAXLEN];

    _size = size;
    _interval = interval;
    _lruCache = new LRU_Cache(size);
    strcpy(_ip, ip);
    strcpy(_master_ip, master_ip);
    _port_to_client = port;
    _master_port = master_port;
    
    strcpy(_ip_port, ip);
    strcat(_ip_port, ":");
    sprintf(_ip_port + strlen(_ip_port), "%d", _port_to_client);
}

Cache::~Cache(){
    delete _ip;
    delete _master_ip;
    delete _buff;
}

void Cache::run_cache(){
    std::thread _thread_listen_to_client(listen_to_client, this);
    std::thread _thread_heart(heart, this);
    _thread_listen_to_client.join();
    _thread_heart.join();
}

// 处理master发来的扩容缩容请求
void Cache::reset_cache(char* recvline){
    int new_size = atoi(recvline);
    _cache_lock.lock();
    _lruCache->reset_size(new_size);
    _cache_lock.unlock();
}

// 完成 client 请求的查询或写入
int Cache::query_cache(char* line){
    std::string recvline = line;
    int result = 0;
    _cache_lock.lock();
    switch (recvline[0])
    {
        case 'w': {
            std::string key = recvline.substr(1, 8);
            std::string str_value = recvline.substr(9);
            int value;
            std::stringstream ss(str_value);
            ss >> value;
            _lruCache->Insert(key, value);
            std::cout << "[Cache] " << "Insert: " << key << '\t' << value << std::endl;
            _lruCache->show();
        }
            break;
        case 'r': {
            std::string key = recvline.substr(1, 8);
            result = _lruCache->Get(key);
            std::cout << "[Cache] " << "Get: " << key << '\t' << result << std::endl;
        }
            break;
        case 's': {
            std::cout << "[Cache] " << "The size is " << _lruCache->GetSize() << std::endl;
            _lruCache->show();
        }
            break;
        default:
            printf("[Cache] Client传入命令出错\n");
            result = -1;
    }
    _cache_lock.unlock();
    return result;
}

// 线程1，接收 client 的查询写入请求，接收 master 的扩容缩容请求
void listen_to_client(Cache* cache){
    SocketServer server_to_client(cache->_ip, cache->_port_to_client);
    char* recvline = nullptr;
    int result;
    while(true){
        recvline = server_to_client.listen_without_close();
        if( recvline[0] != 'r' && recvline[0] != 'w'){
            cache->reset_cache(recvline);
            server_to_client.response_and_close("cache reset size success.");
        }
        else{
            result = cache->query_cache(recvline);
            sprintf(cache->_buff, "%d", result);
            server_to_client.response_and_close(cache->_buff);
        }
    }
}

// 线程2，汇报心跳，给 master 发送自己的 ip 和 port
void heart(Cache* cache){
    char* response;
    while(true){
        response = cache->_client_to_master.send_line(cache->_master_ip, cache->_master_port, cache->_ip_port);
        if (response == nullptr) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else {
            std::this_thread::sleep_for(std::chrono::seconds(cache->_interval));
        }
    }
}
