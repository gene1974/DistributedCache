#include <iostream>
#include <map>  
#include <algorithm>

using namespace std;

struct ListNode
{
	string l_key;
	int l_value;
	ListNode* Pre;
	ListNode* Next;

	ListNode(string key, int value)//初始化
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
	void Insert(string key, int value);// 插入数据
	int Get(string key);
	int GetSize();
	void show();
	void reset_size(int size_num);
private:
	int m_capacity;
	ListNode* pHead;
	ListNode* pTail;
	map<string, ListNode*>  mp;
};

