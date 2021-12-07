#include <iostream>
#include <map>  
#include <algorithm>

using namespace std;

struct ListNode
{
	int l_key;          
	int l_value;
	ListNode* Pre;
	ListNode* Next;

	ListNode(int key, int value);
};

class LRU_Cache
{
public:
	LRU_Cache(int size);
	~LRU_Cache();
	void Remove(ListNode* pNode);	// 移除节点，但不删除节点
	void SetHead(ListNode* pNode);	// 将最近用过的数据要放在队头。
	void Insert(int key, int value);// 插入数据
	int Get(int key);
	int GetSize();
	void show();
private:
	int m_capacity;    
	ListNode* pHead;   
	ListNode* pTail;   
	map<int, ListNode*>  mp;  
};

