#include <iostream>
#include <unistd.h>
#include "client.h"


int main(int argc, char** argv){
    if (argc <= 4){
        printf("[ERROR] Too few arguements.\n");
        printf("[GUIDE] Usage:\n");
        printf("[GUIDE] \t./client ip port master_ip master_port [interval]\n");
        return 0;
    }
    
    char* ip = argv[1];
    int port = atoi(argv[2]);
    char* master_ip = argv[3];
    int master_port = atoi(argv[4]);
    int interval = 5;
    if (argc > 5){
        interval = atoi(argv[5]);
    }
    Client client(ip, port, master_ip, master_port, interval);
    client.run_client();
    return 0;
}
