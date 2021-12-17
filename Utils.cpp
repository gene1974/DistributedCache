#include "Utils.h"

void logger(char* content){
    std::cout << content << std::endl;
    std::cerr << content << std::endl;
}

std::pair<std::string, int> convert_ip(std::string ip_port){
    size_t split = ip_port.find(':');
    int port = atoi(ip_port.c_str() + split + 1);
    ip_port[split] = '\0';
    return std::make_pair(ip_port, port);
}



