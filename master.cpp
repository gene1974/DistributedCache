#include <iostream>
#include <thread>

#include "socket_server.h"
#include "socket_client.h"

// 处理扩容缩容请求
void check_distribution(char* recvline){
    // TODO: 解析从 client 收到的请求 recvline, 完成扩容缩容请求
}

// 完成 client 请求的查询或写入
void write_cache(char* recvline){
    // TODO: 解析从 cache 收到的心跳 recvline, 完成数据的查询或写入
}

// 检查所有 cache 是否正常
bool check_heartpoint(){
    // TODO
    return true;
}

// 重新设置 cache，完成容灾
bool reset_cache(){
    // TODO
    return true;
}

// 线程1，接收 client 的查询分布请求
void listen_to_client(const char* ip, int port){
    //std::cout << "Hello world1!\n";
    SocketServer server(ip, port);
    server.listen_to_port();
}

// 线程2，接收 cache 的心跳信息
void listen_to_cache(const char* ip, int port){
    //std::cout << "Hello world2!\n";
    SocketServer server(ip, port);
    server.listen_to_port();
    
}

// 线程3，整理心跳信息，完成容灾
void check_cache(const char* ip, int port){
    //std::cout << "Hello world3!\n";
    SocketClient client;
    while(true){
        if(!check_heartpoint()){
            reset_cache();
        }
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    
}

int main(int argc, char** argv){
    if (argc <= 4){
        printf("Usage: ./cache $IP $PORT1 $PORT2 $PORT3\n");
        return 0;
    }
    char* ip = argv[1];
    int port1 = atoi(argv[2]);
    int port2 = atoi(argv[3]);
    int port3 = atoi(argv[4]);
    std::thread t1(listen_to_client, ip, port1);
    std::thread t2(listen_to_cache, ip, port2);
    std::thread t3(check_cache, ip, port3);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
