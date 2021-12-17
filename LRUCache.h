#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <iostream>
#include <map>  
#include <algorithm>

struct ListNode
{
	std::string l_key;
	int l_value;
	ListNode* Pre;
	ListNode* Next;

	ListNode(std::string key, int value)//初始化
	{
		l_key = key;
		l_value = value;
		Pre = NULL;
		Next = NULL;
	}
};

class LRU_Cache
{
public:
	LRU_Cache(int size);
	~LRU_Cache();
	void Remove(ListNode* pNode);	// 移除节点，但不删除节点
	void SetHead(ListNode* pNode);	// 将最近用过的数据要放在队头。
	void Insert(std::string key, int value);// 插入数据
	int Get(std::string key);
	int GetSize();
	void show();
	void reset_size(int size_num);
private:
	int m_capacity;
	ListNode* pHead;
	ListNode* pTail;
	std::map<std::string, ListNode*>  mp;
};

#endif 
