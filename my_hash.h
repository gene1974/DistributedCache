#ifndef LEETCODE_MY_HASH_H
#define LEETCODE_MY_HASH_H

#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <vector>

class virtual_node {
public:
    std::string ip;
    unsigned int hash_value;
    std::map<unsigned int, std::string> data;

    virtual_node();

    virtual_node(std::string ip, unsigned int hash_value);

    ~virtual_node();
};

class real_node {
public:
    std::string ip;
    unsigned int virtual_node_num;
    std::vector<unsigned int> virtual_node_hash_list;
    unsigned int cur_max_port;

    real_node();

    real_node(std::string ip);

    ~real_node();
};

class consistent_hash {
public:
    unsigned int real_node_sum;
    unsigned int virtual_node_sum;
    std::map<std::string, real_node> real_node_map;
    std::map<unsigned int, virtual_node> virtual_node_map;
    std::vector<unsigned int> sorted_node_hash_list;

    consistent_hash();

    ~consistent_hash();

    unsigned int find_nearest_node(unsigned int hash_value);

    std::string put(std::string data_id);

    void add_real_node(std::string ip, unsigned int virtual_node_num);

    void drop_real_node(std::string ip);

    void print_real_node(std::string ip);

    void print();

    int get_node_num();
};

#endif //LEETCODE_MY_HASH_H
