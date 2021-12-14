#include <iostream>
#include <thread>
#include  <sstream>
#include  <string>
#include "socket_server.h"

// 处理扩容缩容请求
void reset_cache(char* recvline){
    // TODO: 解析从 master 收到的请求 recvline, 完成扩容缩容请求
}

// 完成 client 请求的查询或写入
void write_cache(string recvline){
    // TODO: 解析从 client 收到的请求 recvline, 完成数据的查询或写入
    
    //LRU_Cache* lruCache = new LRU_Cache(5);这一行放哪儿？？？
    switch (revline[0])
    {
        case 'w': {
            string str_key = revline.substr(1, 9);
            int key;
            stringstream ss(str_key);
            ss >> key;
            string value = revline.substr(9);
            lruCache->Insert(key, value);
        }
            break;
        case 'r': {
            string str_key = revline.substr(1, 9);
            int key;
            stringstream ss(str_key);
            ss >> key;
            lruCache->Get(key);
        }
            break;
        case 's': {
            cout << "The size is " << lruCache->GetSize() << endl;
            lruCache->show();
        }
            break;
        default:
            printf("Client传入命令出错\n");
    }
}

// 线程1，接收 client 的查询写入请求
void listen_to_client(const char* ip, int port){
    //std::cout << "Hello world1!\n";
    SocketServer server(ip, port);
    server.listen_to_port();
}

// 线程2，接收 master 的扩容缩容请求
void listen_to_master(const char* ip, int port){
    //std::cout << "Hello world2!\n";
    SocketServer server(ip, port);
    server.listen_to_port();
}

int main(int argc, char** argv){
    if (argc <= 3){
        printf("Usage: ./cache $IP $PORT1 $PORT2\n");
        return 0;
    }

    char* ip = argv[1];
    int port1 = atoi(argv[2]);
    int port2 = atoi(argv[3]);
    std::thread t1(listen_to_client, ip, port1);
    std::thread t2(listen_to_master, ip, port2);
    t1.join();
    t2.join();
    return 0;
}
