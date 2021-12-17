# DistributedCache

### Using Guide

#### client:
```shell
# compile
g++ run_client.cpp client.cpp socket_client.cpp socket_server.cpp Utils.cpp -lpthread -o client

#run
./client ip port master_ip master_port [interval]
```

### 系统框架

搭了一下系统框架，按照我的想法大概写了一下需要的线程和函数，具体内容后续完善。

```Shell
g++ master.cpp socket_server.cpp socket_client.cpp -lpthread -o master && ./master ip port1 port2 port3

g++ client.cpp socket_server.cpp socket_client.cpp -lpthread -o client && ./client ip1 port1 ip2 port2

g++ cache.cpp socket_server.cpp socket_client.cpp -lpthread -o cache && ./cache ip port1 port2
```

### Socket 通信

接口示例：

Client:

```C++
#include "socket_client.h"

// 建立 socket client：
SocketClient socket_client();
// 连接 server 并发送一条信息：
socket_client.connect_and_send("127.0.0.1", 6666, "Hello World!")
// 循环从 stdin 读取输入，并发送给 server
socket_client.send_stdin("127.0.0.1", 6666);
```

Server:

```C++
#include "socket_server.h"

// 建立 socket client：
SocketServer socker_server("127.0.0.1", 6666);
// 循环监听端口信息：
socker_server.listen_to_port();
```

Demo运行方法：

Terminal 1：
```Shell
g++ socket_server.cpp -o socket_server && ./socket_server
```

Terminal 2：
```Shell
g++ socket_client.cpp -o socket_client && ./socket_client
```