# DistributedCache

### Design Document

https://docs.qq.com/doc/DZmF1RExManVORlpr

### Using Guide

#### client:
```shell
# compile
g++ run_client.cpp client.cpp socket_client.cpp socket_server.cpp Utils.cpp -lpthread -o client

#run
./client ip port master_ip master_port [interval]
```

#### master:
```shell
# compile
g++ run_master.cpp master.cpp my_hash.cpp socket_client.cpp socket_server.cpp Utils.cpp -lpthread -o master

#run
./master ip port1 port2 client_ip client_port [interval]
```

#### cache:
```shell
# compile
g++ run_cache.cpp cache.cpp LRUCache.cpp socket_client.cpp socket_server.cpp Utils.cpp -lpthread -o cache

#run
./cache ip port _master_ip _master_port [size]
```
