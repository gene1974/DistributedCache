#include "conHash.h"

conHash::conHash() {
	_murMurHash = murMurHash();
}

conHash::~conHash() {
	for(auto iter: mapVirtualNodes)
		delete iter.second;
	mapHashNodes.clear();
	mapVirtualNodes.clear();
}

// 添加一个节点
bool conHash::addNode(hashNode* node) {

	string name = node->name;
	node->hashCode = getHashCode(name);	//计算获得相应哈希值

	// 添加真实节点
	pair< map<string, hashNode*>::iterator, bool> result = mapHashNodes.insert(pair<string, hashNode*>(node->hashCode, node));

	/**
	*	添加其虚拟节点
	*	自己也看成一个虚拟节点
	**/
	virtualNode* vn = new virtualNode(node->name, node->name);
	mapVirtualNodes.insert(pair<string, virtualNode*>(node->hashCode, vn));

	int numOfVirtualhashNode = node->numOfVirtualhashNode;
	for(int i = 0; i < numOfVirtualhashNode - 1; ++i) {
		char buffer[20];
		sprintf(buffer, ":%04d", i);
		string virNodeName = name + buffer;
		virtualNode* vn = new virtualNode(name, virNodeName);
		vn->hashCode = getHashCode(virNodeName.substr(6));
		mapVirtualNodes.insert(pair<string, virtualNode*>(vn->hashCode, vn));
	}

	if(result.second)
		cout << "Add the service node: " << node->name << " and corresponding hash code : " + node->hashCode << endl;

	return (bool)result.second;
}

// 删除一个节点
bool conHash::delNode(hashNode* node) {
	string serviceName = node->name;
	string key = node->hashCode;
	auto iter = mapHashNodes.find(key);
	if(iter != mapHashNodes.end()) {
		mapHashNodes.erase(iter);
		delete node;
		node = NULL;
		cout << "Delete the service node: " << serviceName << " and corresponding hash code : " << key << endl;
		for(auto virIter = mapVirtualNodes.begin(); virIter != mapVirtualNodes.end(); ) {  
			if(virIter->second->corrNodeName == serviceName) {
				virtualNode* vnode = virIter->second;
				mapVirtualNodes.erase(virIter++);  
				delete vnode;
			}
			else  
				++virIter;  
		} 
		return true;
	}
	
	return false;
}

// 通过哈希码查找一个服务节点
hashNode* conHash::find(string hashKey){
	auto iter = mapHashNodes.find(hashKey);
	if(iter != mapHashNodes.end())
		return iter->second;
	return NULL;
}

// 计算得到相应一个哈希值
string conHash::getHashCode(string str) {
	return to_string(_murMurHash.getMurMurHash(&str, keyLen));
}

// 判断当前节点是否为0
bool conHash::empty() {
	return mapHashNodes.empty();
}

// 得到应当路由到的结点
// 此处的node指需要被进行处理的数据
string conHash::getService(string node) {
	string hashCode = getHashCode(node);	// 计算获得相应哈希值
	string route;
	for(auto iter = mapVirtualNodes.begin(); iter != mapVirtualNodes.end(); iter++) {
        if(iter->first >= hashCode) {
        	route = node + " is routed to the virtual node " + iter->second->virNodeName + " and the real node is " + iter->second->corrNodeName + "\n";
        	for(auto i : mapHashNodes)
        		if(i.second->name == iter->second->corrNodeName) {
        			i.second->routeService++;
        			break;
        		}
        	break;
        }
    }
    return route;
}

// 获得真实节点的个数
int conHash::getServiceNodeNum() {
	return mapHashNodes.size();
}

// 获得虚拟节点的个数
int conHash::getVirtualNodeNum() {
	return mapVirtualNodes.size();
}

// 显示当前服务器路由情况
void conHash::showService() {

	cout << "Current situation: " << endl;

	if(empty())
		cout << "There are no service node now" << endl;
	else {
		for(auto iter : mapHashNodes)
			cout << "The service node: " << iter.second->name << " and the corresponding hash code: " << iter.second->hashCode << " provides " << iter.second->routeService << " service" << endl;
	}
}