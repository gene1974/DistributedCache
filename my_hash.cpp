#include "my_hash.h"

int HASH_LEN = 32;

unsigned int my_getMurMurHash(const void *key, int len) {
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    const int seed = 97;
    unsigned int h = seed ^len;
    // Mix 4 bytes at a time into the hash
    const unsigned char *data = (const unsigned char *) key;
    while (len >= 4) {
        unsigned int k = *(unsigned int *) data;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        data += 4;
        len -= 4;
    }
    // Handle the last few bytes of the input array
    switch (len) {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    };
    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}

virtual_node::virtual_node(std::string ip, unsigned int hash_value) {
    this->ip = ip;
    this->hash_value = hash_value;
}

virtual_node::~virtual_node() {
    data.clear();
}

virtual_node::virtual_node() {
    this->ip = "";
    this->hash_value = 0;
}


real_node::real_node(std::string ip) {
    this->ip = ip;
    this->cur_max_port = 0;
    this->virtual_node_num = 0;
}

real_node::~real_node() {
    this->virtual_node_hash_list.clear();
}

real_node::real_node() {
    this->ip = "";
    this->cur_max_port = 0;
    this->virtual_node_num = 0;
}

consistent_hash::consistent_hash() {
    this->real_node_sum = 0;
    this->virtual_node_sum = 0;
}

consistent_hash::~consistent_hash() {
    this->virtual_node_map.clear();
    this->real_node_map.clear();
    this->sorted_node_hash_list.clear();
}

unsigned int consistent_hash::find_nearest_node(unsigned int hash_value) {
    int low = 0;
    int high = this->sorted_node_hash_list.size() - 1;
    int mid;
    if (hash_value > this->sorted_node_hash_list[high]) {
        return 0;
    }
    while (low < high) {
        mid = (low + high) / 2;
        if (this->sorted_node_hash_list[mid] == hash_value) {
            return mid;
        } else if (this->sorted_node_hash_list[mid] > hash_value) {
            high = mid;
        } else {//this->sorted_node_hash_list[mid]<data_hash
            low = mid + 1;
        }
    }
    return low;
}

void consistent_hash::add_real_node(std::string ip, unsigned int virtual_node_num = 300) {
    std::cout << "[Master] " << "[add_real_node]\t" << ip << std::endl;
    real_node *node;
   
    if (this->real_node_map.find(ip) != this->real_node_map.end()) {
        node = &real_node_map[ip];
    } else {
        real_node *new_node =new real_node(ip);
        node = new_node;
        this->real_node_sum++;
    }

    int cur_port = node->cur_max_port;
    int vir_node_num = 0;
    std::string tmp_ip;
    unsigned int tmp_hash;
    
    while (vir_node_num < virtual_node_num) {
        do {
            cur_port++;
            tmp_ip = ip + ":" + std::to_string(cur_port);
            tmp_hash = my_getMurMurHash(tmp_ip.c_str(), HASH_LEN);
        } while (this->virtual_node_map.find(tmp_hash) != this->virtual_node_map.end());
        
        vir_node_num++;
        this->virtual_node_map[tmp_hash] = virtual_node(tmp_ip, tmp_hash);
        this->sorted_node_hash_list.push_back(tmp_hash);
        sort(this->sorted_node_hash_list.begin(), this->sorted_node_hash_list.end());
        unsigned int id = this->find_nearest_node(tmp_hash);
        unsigned int next_id = id + 1;
        if (next_id >= this->sorted_node_hash_list.size()) {
            next_id = 0;
        }
       
        unsigned int next_hash = this->sorted_node_hash_list[next_id];
        std::vector<unsigned int> tobe_deleted;
        std::map<unsigned int, std::string> *tobe_robbed = &(this->virtual_node_map[next_hash].data);
        
        for (auto data = tobe_robbed->begin(); data != tobe_robbed->end(); data++) {
            if (data->first < tmp_hash) {
                this->virtual_node_map[tmp_hash].data[data->first] = data->second;
                tobe_deleted.push_back(data->first);
            }
        }
        
        for (auto deleted:tobe_deleted) {
            tobe_robbed->erase(deleted);
            std::cout << "[move data]\t" << deleted << "\tfrom node:\t" << this->virtual_node_map[next_hash].ip << "("
                 << next_hash << ")" << "\tto\t"
                 << this->virtual_node_map[tmp_hash].ip << "(" << tmp_hash << ")" << std::endl;
        }
        
        node->virtual_node_hash_list.push_back(tmp_hash);
        
        //std::cout << "2" << std::endl;
    }
    
    node->cur_max_port = cur_port;
    node->virtual_node_num += virtual_node_num;
    this->real_node_map[ip] = *node;

    this->virtual_node_sum += virtual_node_num;
    std::cout << "[Master] " << "[add_real_node finished]\t" << ip << std::endl;
}


void consistent_hash::print_real_node(std::string ip) {
    std::cout << "------------consistent_hash.print_real_node------------" << std::endl;
    std::cout << "real_node ip:" << ip << "\tvirtual_node_num=" << this->real_node_map[ip].virtual_node_num << std::endl;
    for (auto tmp:this->real_node_map[ip].virtual_node_hash_list) {
        if (this->virtual_node_map[tmp].data.size() > 0) {
            std::cout << "virtual node:\t" << this->virtual_node_map[tmp].ip << "(" << tmp << ")" << "\thas data:";
            for (auto data:this->virtual_node_map[tmp].data) {
                std::cout << "(" << data.second << "," << data.first << ")\t";
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void consistent_hash::print() {
    std::cout << std::endl << "------------consistent_hash.print()------------" << std::endl;
    std::cout << "real_node_sum:\t" << this->real_node_sum << "\tvirtual_node_sum:\t" << this->virtual_node_sum << std::endl;
    std::cout << std::endl;
    for (auto tmp = this->real_node_map.begin(); tmp != this->real_node_map.end(); tmp++) {
        this->print_real_node(tmp->first);
    }

}


std::string consistent_hash::put(std::string data_id) {
    unsigned int data_hash = my_getMurMurHash(data_id.c_str(), HASH_LEN);
    unsigned int id = this->find_nearest_node(data_hash);
    unsigned int put_on_virnode_hash = this->sorted_node_hash_list[id];
    this->virtual_node_map[put_on_virnode_hash].data.insert(make_pair(data_hash, data_id));
    std::cout << "[Master] " << "data:\t" << data_id << "(" << data_hash << ")\twas put on virtual node:"
         << this->virtual_node_map[put_on_virnode_hash].ip << "(" << put_on_virnode_hash << ")"
         << std::endl;
    std::string vir_ip = this->virtual_node_map[put_on_virnode_hash].ip;
    size_t pos = vir_ip.find_last_of(':');
    return vir_ip.substr(0, pos);
}

void consistent_hash::drop_real_node(std::string ip) {
    std::cout << "[Master] " << "[drop_real_node]\t" << ip << std::endl;
    std::vector<unsigned int> *virtual_hash_list_p = &this->real_node_map[ip].virtual_node_hash_list;
    std::sort(virtual_hash_list_p->begin(), virtual_hash_list_p->end());
    unsigned int next_id;
    unsigned int next_hash;
    unsigned int cur_id;
    unsigned int cur_hash;
    std::vector<unsigned int> tobe_delete;
    for (int i = virtual_hash_list_p->size() - 1; i >= 0; i--) {
        cur_hash = (*virtual_hash_list_p)[i];
        tobe_delete.push_back(cur_hash);
        if (this->virtual_node_map[cur_hash].data.size() > 0) {
            cur_id = this->find_nearest_node(cur_hash);
            next_id = cur_id;
            std::string next_realnode_ip;
            do {
                next_id++;
                if (next_id >= this->sorted_node_hash_list.size()) {
                    next_id = 0;
                }
                next_hash = this->sorted_node_hash_list[next_id];
                next_realnode_ip = this->virtual_node_map[next_hash].ip;
            } while (next_realnode_ip.find(ip) != -1);
            std::map<unsigned int, std::string> *moveto = &(this->virtual_node_map[next_hash].data);
            for (auto &data : this->virtual_node_map[cur_hash].data) {
                (*moveto)[data.first] = data.second;
                std::cout << "[Master] " << "[move data]\t" << data.second << "\tfrom node:\t" << this->virtual_node_map[cur_hash].ip << "("
                     << cur_hash << ")" << "\tto\t"
                     << this->virtual_node_map[next_hash].ip << "(" << next_hash << ")" << std::endl;
            }
        }
    }
    for (auto hash:tobe_delete) {
        this->virtual_node_map.erase(cur_hash);
        this->virtual_node_sum--;
        auto iter = find(this->sorted_node_hash_list.begin(),
                         this->sorted_node_hash_list.end(), hash);
        if (iter != this->sorted_node_hash_list.end()) {
            this->sorted_node_hash_list.erase(iter);
        }
    }
    sort(this->sorted_node_hash_list.begin(), this->sorted_node_hash_list.end());
    this->real_node_map[ip].virtual_node_hash_list.clear();
    this->real_node_map.erase(ip);
    this->real_node_sum--;
    std::cout << "[Master] " << "[drop_real_node finished]\t" << ip << std::endl << std::endl;
}

int consistent_hash::get_node_num(){
    return real_node_sum;
}
