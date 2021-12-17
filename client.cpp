
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
    _port_to_master = port;
    _master_port = master_port;
    _interval = interval;
    _is_write = true;
}

Client::~Client(){
    delete _ip;
    delete _master_ip;
}

std::string Client::get_distribution(std::string key){
    std::string cache_string;
    _hash_lock.lock();
    if (_local_hash.count(key) != 0) { // in local list
        cache_string = _local_hash[key];
    }
    else {
        cache_string = request_master(key); // request master
        _local_hash[key] = cache_string; // update local list
    }
    
    std::cout << "[Client] Local hash: " << std::endl;
    for(auto iter = _local_hash.begin(); iter != _local_hash.end(); iter++){
        std::cout << iter->first << '\t' << iter->second << std::endl;
    }
    std::cout << std::endl;

    _hash_lock.unlock();
    return cache_string;
}

std::string Client::request_master(std::string sendline){
    char* recvline = _socket_client.send_line(_master_ip, _master_port, sendline.c_str());
    if (recvline != nullptr){
        return recvline;
    }
    return "";
}

void Client::clear_local(){
    _hash_lock.lock();
    _local_hash.clear();
    _hash_lock.unlock();
}

char* Client::request_cache(const char* ip, int port, std::string data){
    auto recvline = _socket_client.send_line(ip, port, data.c_str());
    return recvline;
}

void Client::run_client(){
    std::thread thread_listen_to_master(listen_to_master, this);
    std::string cache_string; // ip + port
    std::pair<std::string, int> cache;
    std::pair<std::string, int> data;
    std::string sendline;
    int cnt;
    sleep(10); // wait master and cache setup
    while(true){
        data = gendata();
        cache_string = get_distribution(data.first);
        cache = convert_ip(cache_string);
        
        if (_is_write) {
            sendline = 'w' + data.first + std::to_string(data.second);
        }
        else {
            sendline = 'r' + data.first + std::to_string(data.second);
        }
        request_cache(cache.first.c_str(), cache.second, sendline);
        sleep(_interval);
        cnt++;
        if(cnt == 3) {
            sendline = 'r' + data.first;
            request_cache(cache.first.c_str(), cache.second, sendline);
            cnt = 0;
            sleep(_interval);
        }
    }
    thread_listen_to_master.join();
}

void listen_to_master(Client* client){
    SocketServer server_to_master(client->_ip, client->_port_to_master);
    while(true){
        char* recvline = server_to_master.listen_once();
        if(strcmp(recvline, "clear") == 0) {
            std::cout << "[Client] Clear local hash." << std::endl;
            client->clear_local();
        }
    }
}
