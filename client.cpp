
#include "client.h"

std::string strRand(int length = 8) {			// length: 产生字符串的长度
    char tmp;							// tmp: 暂存一个随机数
    std::string buffer;						// buffer: 保存返回值

   
    std::random_device rd;					// 产生一个 std::random_device 对象 rd
    std::default_random_engine random(rd());	// 用 rd 初始化一个随机数发生器 random

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
    std::random_device rd;  // 将用于为随机数引擎获得种子
    std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
    std::uniform_int_distribution <int> dis(MIN, MAX);
    // 用 dis 变换 gen 所生成的随机 unsigned int 到 [MIN, MAX] 中的 int
    return dis(gen);
}

std::pair<std::string, int>  gendata() {
    return make_pair(strRand(), keyRand());
}

Client::Client(const char* ip, int port, const char* master_ip, int master_port, time_t interval = 1){
    _ip = new char[20];
    _master_ip = new char[20];
    buff = new char[MAXLEN];
    
    strcpy(_ip, ip);
    strcpy(_master_ip, master_ip);
    _port = port;
    _master_port = master_port;
    _interval = interval;
    _is_write = true;
}

Client::~Client(){
    delete _ip;
    delete _master_ip;
}

std::string Client::request_master(std::string sendline){
    char* recvline = _socket_client.send_line(_master_ip, _master_port, sendline.c_str());
    if (recvline != nullptr){
        return recvline;
    }
    return "";
}

bool Client::write_local(std::string key, std::string cache_string){
    _local_hash[key] = cache_string;
    return true;
}

void Client::clear_local(){
    _local_hash.clear();
}

char* Client::request_cache(const char* ip, int port, std::string data){
    auto recvline = _socket_client.send_line(_master_ip, _master_port, data.c_str());
    return recvline;
}

void Client::run_client(){
    std::thread thread_listen_to_master(listen_to_master, this);
    std::string cache_string; // ip + port
    std::pair<std::string, int> cache;
    std::pair<std::string, int> data;
    std::string sendline;
    while(true){
        data = gendata();
        if (_local_hash.count(data.first) != 0) { // in local list
            cache_string = _local_hash[data.first];
        }
        else {
            cache_string = request_master(data.first); // request master
            write_local(data.first, cache_string); // update local list
        }
        
        if (_is_write) {
            sendline = 'w' + data.first + std::to_string(data.second);
        }
        else {
            sendline = 'r' + data.first + std::to_string(data.second);
        }
        cache = convert_ip(cache_string);
        request_cache(cache.first.c_str(), cache.second, sendline);
        sleep(_interval);
    }
    thread_listen_to_master.join();
}

void listen_to_master(Client* client){
    SocketServer server_to_master(client->_ip, client->_port);
    while(true){
        char* recvline = server_to_master.listen_once();
        if(strcmp(recvline, "clear") == 0) {
            client->clear_local();
        }
    }
}




// int main(){
    
//     SingleLinkedList<string> list;
//     list.append(*gendata(1), "1");
//     list.append(*gendata(1), "1");
//     list.append(*gendata(1), "2");
//     list.append(*gendata(1), "3");
//     list.append(*gendata(1), "0");
//     list.print();
//     cout << list.length() << endl;
    

//     return 0;

// }