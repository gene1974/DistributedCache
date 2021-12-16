#include <ctime>
#include <vector>
#include <string>
#include "socket_client.h"
#include "socket_server.h"
#include "my_hash.h"

class Master{
public:
    Master(const char* ip, int port1, int port2, unsigned interval);
    ~Master();
    void run_master();
    void listen_to_client();
    void listen_heart();
    void send_to_cache();
    
    void check_cache();
    void check_heartpoint();
    void remove_cache(std::string bad_cache);
    void reset_cache();

private:
    char* _ip;
    int _port_to_client;
    int _port_to_cache;
    
    // std::vector<char*> _server_ip;
    // std::vector<int> _server_port;
    SocketClient _client_to_cache;

    time_t _interval;
    consistent_hash _hash;
    std::map<std::string, time_t> _last_time;
};