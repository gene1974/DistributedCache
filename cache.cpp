#include <iostream>
#include <thread>
#include  <sstream>
#include  <string>
#include "cache.h"

Cache::Cache(const char* ip, int port1, int port2, const char* master_ip, int master_port, int size){
    _ip = new char[20];
    _master_ip = new char[20];
    _buff = new char[MAXLEN];

    _size = size;
    _lruCache = new LRU_Cache(size);
    strcpy(_ip, ip);
    strcpy(_master_ip, master_ip);
    _port_to_client = port1;
    _port_to_master = port2;
    _master_port = master_port;
}

Cache::~Cache(){
    delete _ip;
    delete _master_ip;
    delete _buff;
}

void Cache::run_cache(){
    std::thread _thread_listen_to_client(listen_to_client);
    std::thread _thread_listen_to_master(listen_to_master);
    std::thread _thread_heart(heart);
    _thread_listen_to_client.join();
    _thread_listen_to_master.join();
    _thread_heart.join();
}

// 处理master发来的扩容缩容请求
void Cache::reset_cache(char* recvline){
    int new_size = atoi(recvline);
    _lruCache->reset_size(new_size);
}

// 完成 client 请求的查询或写入
void Cache::write_cache(char* line){
    std::string recvline = line;
    switch (recvline[0])
    {
        case 'w': {
            std::string str_key = recvline.substr(1, 9);
            int key;
            std::stringstream ss(str_key);
            ss >> key;
            std::string value = recvline.substr(9);
            _lruCache->Insert(key, value);
        }
            break;
        case 'r': {
            std::string str_key = recvline.substr(1, 9);
            int key;
            std::stringstream ss(str_key);
            ss >> key;
            _lruCache->Get(key);
        }
            break;
        case 's': {
            std::cout << "The size is " << _lruCache->GetSize() << std::endl;
            _lruCache->show();
        }
            break;
        default:
            printf("Client传入命令出错\n");
    }
}

// 线程1，接收 client 的查询写入请求
void Cache::listen_to_client(){
    SocketServer _server_to_client(_ip, _port_to_client);
    while(true){
        char* recvline = _server_to_client.listen_once();
        write_cache(recvline);
    }
    
}

// 线程2，接收 master 的扩容缩容请求
void Cache::listen_to_master(){
    SocketServer _server_to_master(_ip, _port_to_master);
    while(true){
        char* recvline = _server_to_master.listen_once();
        reset_cache(recvline);
    }
}


// 线程3，汇报心跳
void Cache::heart(){
    //TODO
    _client_to_master.send_line(_master_ip, _master_port, "heart");
    
    
}

// int main(int argc, char** argv){
//     if (argc <= 3){
//         printf("Usage: ./cache $IP $PORT1 $PORT2\n");
//         return 0;
//     }

//     char* ip = argv[1];
//     int port1 = atoi(argv[2]);
//     int port2 = atoi(argv[3]);
//     std::thread t1(listen_to_client, ip, port1);
//     std::thread t2(listen_to_master, ip, port2);
//     t1.join();
//     t2.join();
//     return 0;
// }
