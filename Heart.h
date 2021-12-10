#include<ctime>
#include<iostream>
#include<string.h>
#include<winsock2.h>

using namespace std;

class Heart{
public:
    Heart();

    //server
    bool HeartSend(SOCKET sock);

    //master
    bool HeartCheck(SOCKET sock);

private:
    char m_heart[32];//master buff
    char s_heart[32];//server buff

    size_t start;//thread init time & old heart time
    size_t stop;//new heart time
    size_t h_interval;

}