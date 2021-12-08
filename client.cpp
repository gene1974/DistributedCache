#include <iostream>
#include <map> 
#include <random>
#include <string>  
#include <thread>

using std::string;
using std::random_device;
using std::default_random_engine;

string strRand(int length = 8) {			// length: 产生字符串的长度
    char tmp;							// tmp: 暂存一个随机数
    string buffer;						// buffer: 保存返回值

   
    random_device rd;					// 产生一个 std::random_device 对象 rd
    default_random_engine random(rd());	// 用 rd 初始化一个随机数发生器 random

    for (int i = 0; i < length; i++) {
        tmp = random() % 36;	// 随机一个小于 36 的整数，0-9、A-Z 共 36 种字符
        if (tmp < 10) {			// 如果随机数小于 10，变换成一个阿拉伯数字的 ASCII
            tmp += '0';
        }
        else {				// 否则，变换成一个大写字母的 ASCII
            tmp -= 10;
            tmp += 'A';
        }
        buffer += tmp;
    }
    return buffer;
}

int keyRand() {

    constexpr int MIN = 10000000;
    constexpr int MAX = 99999999;
    random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(MIN, MAX);
    return distr(eng);
}


char* generate_key(){
    char* key = nullptr;
    // TODO: generate key

    return key;
}

std::pair<char*, int> generate_key_value(){
    char* key = nullptr;
    int value = 0;
    // TODO: generate key and value
    
    return std::make_pair(key, value);
}

char* request_master(const char* ip, int port){
    // TODO: send key to master, get distribution
    return NULL;
}

bool write_distribution(){
    // TODO: save distribution locally
    return true;
}

char* request_cache(const char* ip, int port){
    // TODO: send (key, value) to cache server
    return NULL;
}

void client(const char* ip1, int port1, const char* ip2, int port2){
    std::pair<char*, int> p;
    while(true){
        generate_key_value();
        request_master(ip1, port1);
        write_distribution();
        request_cache(ip2, port2);
    }
}

int main(int argc, char** argv){
    if (argc <= 4){
        printf("Usage: ./cache $IP1 $PORT1 $IP2 $PORT2\n");
        return 0;
    }
    char* ip1 = argv[1];
    int port1 = atoi(argv[2]);
    char* ip2 = argv[3];
    int port2 = atoi(argv[4]);
    client(ip1, port1, ip2, port2);
    return 0;
}
