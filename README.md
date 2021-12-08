# DistributedCache

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
g++ socket_server.cpp -o server && ./server
```

Terminal 2：
```Shell
g++ socket_client.cpp -o client && ./client
```