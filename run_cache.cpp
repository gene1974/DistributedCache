#include <iostream>
#include <unistd.h>
#include "cache.h"


int main(int argc, char** argv){
    if (argc <= 5){
        printf("[ERROR] Too few arguements.\n");
        printf("[GUIDE] Usage:\n");
        printf("[GUIDE] \t./cache ip port1 port2 _master_ip _master_port [size]\n");
        return 0;
    }
    
    char* ip = argv[1];
    int port1 = atoi(argv[2]);
    int port2 = atoi(argv[3]);
    char* _master_ip = argv[4];
    int _master_port = atoi(argv[5]);
    int size = 5;
    int interval = 20;
    if (argc > 6){
        size = atoi(argv[6]);
    }
    if (argc > 7){
        interval = atoi(argv[7]);
    }
    Cache cache(ip, port1, port2, _master_ip, _master_port, size, interval);
    cache.run_cache();
    return 0;
}
