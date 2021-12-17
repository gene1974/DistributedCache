#include <iostream>
#include <unistd.h>
#include "cache.h"


int main(int argc, char** argv){
    if (argc <= 4){
        printf("[ERROR] Too few arguements.\n");
        printf("[GUIDE] Usage:\n");
        printf("[GUIDE] \t./cache ip port _master_ip _master_port [size]\n");
        return 0;
    }
    
    char* ip = argv[1];
    int port = atoi(argv[2]);
    char* _master_ip = argv[3];
    int _master_port = atoi(argv[4]);
    int size = 5;
    int interval = 20;
    if (argc > 5){
        size = atoi(argv[5]);
    }
    if (argc > 6){
        interval = atoi(argv[6]);
    }
    Cache cache(ip, port, _master_ip, _master_port, size, interval);
    cache.run_cache();
    return 0;
}
