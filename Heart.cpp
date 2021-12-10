#include"Heart.h"

Heart::Heart(){

}

//server
bool Heart::HeartSend(SOCKET sock){
//好像不需要参数, 因为只有一个master 写死就行?
    memset(s_heart, 0, 32);
    snprintf(s_heart, 32, "bangbang5");

    if(send(sock, s_heart, 32, 0) < 0){
        perror("send error");
        return false;
    }
    memset(s_heart, 0, 32);

    if(recv(sock, s_heart, 32, 0) < 0){
        perror("recv error");
        return false;
    }

    return true;
}

//master
bool Heart::HeartCheck(SOCKET sock){
    start = clock();

    int heart = recv(sock, m_heart, 32, 0);

    if( heart > 0){//收到消息
        //执行函数
        start = clock();
        snprintf(m_heart, 32, "ACCEPT HEART");
        send(sock, m_heart, 32, 0);//回复已经收到
    }else{//没有消息
        size_t stop = clock();
        h_interval = stop - start;
        if((h_interval) > 60000){
            //大于60s无数据
            ReduceServer();
            closesocket(sock);
            closethread();
        }
        } 
    return true;
}  
