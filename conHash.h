#ifndef __CON_HASH_H_
#define __CON_HASH_H_ 

#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include "murMurHash.h"

using namespace std;

#define keyLen 32

/* 虚拟节点的结构定义 */
struct virtualNode {
	string corrNodeName;
	string virNodeName;
	string hashCode;

	virtualNode(string _corrNodeName, string _virNodeName) {
		corrNodeName = _corrNodeName;
		virNodeName = _virNodeName;
	}

	string toString() {
		string str = "The virtual node: " + virNodeName + " is a virtual node of the real node: " + corrNodeName;
		return str;
	}	
};

/* 真实节点的结构定义 */
struct hashNode {
	string name;						// 节点标记
	string hashCode;					// 节点的哈希值
	int numOfVirtualhashNode;			// 虚拟节点的个数
	int routeService;					// 记录负载的路由			

	// 初始化
	hashNode(string _name, int _numOfVirtualhashNode) {
		name = _name;
		numOfVirtualhashNode = _numOfVirtualhashNode + 1;
		routeService = 0;
	}
};

class conHash {

public:

	conHash();
	
	~conHash();

	// 添加一个节点
	bool addNode(hashNode* node);

	// 删除一个节点
	bool delNode(hashNode* node);

	// 通过哈希码查找一个服务节点
	hashNode* find(string hashKey);

	// 计算得到相应一个哈希值
	string getHashCode(string str);

	// 判断当前节点是否为0
	bool empty();

	// 得到应当路由到的结点
	string getService(string node);

	// 获得真实节点的个数
	int getServiceNodeNum();

	// 获得虚拟节点的个数
	int getVirtualNodeNum();

	// 显示当前服务器路由情况
	void showService();
private:
	
	murMurHash _murMurHash;

	/*
	*	管理真实节点
	* 	first为节点的哈希值，second为节点 
	*/ 
	map<string, hashNode*> mapHashNodes;

	/*
	*	管理所有虚拟节点
	* 	first为节点的哈希值，second为节点 
	*/
	map<string, virtualNode*> mapVirtualNodes;

};

#endif