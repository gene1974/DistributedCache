#include <iostream>
#include <unistd.h>
#include "master.h"


int main(int argc, char** argv){
    if (argc <= 5){
        printf("[ERROR] Too few arguements.\n");
        printf("[GUIDE] Usage:\n");
        printf("[GUIDE] \t./master ip port1 port2 client_ip client_port [interval]\n");
        return 0;
    }
    
    char* ip = argv[1];
    int port1 = atoi(argv[2]);
    int port2 = atoi(argv[3]);
    char* client_ip = argv[4];
    int client_port = atoi(argv[5]);
    int interval = 60;
    if (argc > 6){
        interval = atoi(argv[6]);
    }
    Master master(ip, port1, port2, client_ip, client_port, interval);
    master.run_master();
    return 0;
}