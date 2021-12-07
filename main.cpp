/**
 *	测试 
 */


#include "conHash.h"
#include <sstream>
#include <fstream>
using namespace std;

//产生随机IP地址
string RandIP();

int main() {
	
	printf("A simple test of Consistent Hashing\n");

	conHash _conhash = conHash();

	cout << "-----------------ADD SERVICE NODES----------------------" << endl;
	// 创建服务器节点
	hashNode* node_0 = new hashNode("192.168.127.140", 30);
	hashNode* node_1 = new hashNode("192.168.127.141", 30);
	hashNode* node_2 = new hashNode("192.168.127.142", 30);

	// 加入服务器节点
	_conhash.addNode(node_0);
	_conhash.addNode(node_1);
	_conhash.addNode(node_2);

	
	// 分别输出真实和虚拟服务节点的个数
	cout << "The total num of service node: " << _conhash.getServiceNodeNum() << endl;
	cout << "The total num of virtual service node: " << _conhash.getVirtualNodeNum() << endl;

	cout << "-----------------------TEST--------------------------" << endl;
	// 模拟请求
	for(int i = 0; i < 10000; ++i) 
		cout << _conhash.getService(RandIP());


	cout << "-------------------SHOW SERVICE----------------------" << endl;
	// 查看每个节点负责的请求
	_conhash.showService();


	// 删除某个服务节点
	cout << "----------------------DELETE-------------------------" << endl;
	_conhash.delNode(node_0);
	cout << "The total num of service node: " << _conhash.getServiceNodeNum() << endl;
	cout << "The total num of virtual service node: " << _conhash.getVirtualNodeNum() << endl;


	delete node_1;
	delete node_2;
	
	return 0;
}

//产生随机IP地址
string RandIP()
{
	stringstream ip;
	for (int i = 0; i < 4; ++i) {
		ip << (rand() % 256);
		if (i < 3)
			ip << '.';
	}

	return ip.str();
}