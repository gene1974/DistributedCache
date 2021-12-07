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

	ListNode(int key, int value)//初始化
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
	LRU_Cache(int size)//构造函数
	{
		m_capacity = size;
		pHead == NULL;
		pTail == NULL;
	}

	~LRU_Cache()//析构函数
	{
		map<int, ListNode*>::iterator it = mp.begin();
		for (; it != mp.end();)
		{
			delete it->second;
			it->second = NULL;
			mp.erase(it++);  
		}
		delete pHead;
		pHead == NULL;
		delete pTail;
		pTail == NULL;

	}
	//移除节点，但不删除节点
	void Remove(ListNode* pNode)
	{
		// 如果是头节点
		if (pNode->Pre == NULL)
		{
			pHead = pNode->Next;
			pHead->Pre = NULL;
		}

		// 如果是尾节点
		if (pNode->Next == NULL)
		{
			pTail = pNode->Pre;
			pTail->Next = NULL;
		}

		else
		{
			pNode->Pre->Next = pNode->Next;
			pNode->Next->Pre = pNode->Pre;
		}

	}
	//  将最近用过的数据要放在队头。
	void SetHead(ListNode* pNode)
	{
		pNode->Next = pHead;
		pNode->Pre = NULL;
		if (pHead == NULL)
		{
			pHead = pNode;
		}
		else
		{
			pHead->Pre = pNode;
			pHead = pNode;

		}
		if (pTail == NULL)
		{
			pTail = pHead;
		}
	}
	//插入数据
	void Insert(int key, int value)
	{
		map<int, ListNode*>::iterator it = mp.find(key);//如果新key等于旧key，则更新value
		if (it != mp.end())
		{
			ListNode* Node = it->second;
			Node->l_value = value;
			Remove(Node);
			SetHead(Node);
		}
		else
		{
			ListNode* NewNode = new ListNode(key, value);
			if (mp.size() >= m_capacity)
			{
				cout << endl << "警告：已经超出最大容量:" << m_capacity << "个数据" << endl;
				int x = pTail->l_key;
				map<int, ListNode*>::iterator it = mp.find(x);
				Remove(pTail); //移除表尾指针内存
				int y = it->second->l_value;
				delete it->second;//删除value
				cout << endl << "为您移除key为" << x << ",value为" << y << "的节点" << endl;
				mp.erase(it);
			}
			SetHead(NewNode);//放到头部
			mp[key] = NewNode;
		}
	}

	int Get(int key)
	{
		map<int, ListNode*>::iterator it = mp.find(key);
		if (it != mp.end())
		{
			ListNode* Node = it->second;
			Remove(Node);
			SetHead(Node);
			return Node->l_value;
		}
		else
		{
			return -1;     //有待商讨
		}
	}

	int GetSize()
	{
		return mp.size();
	}

	void show() {
		int i = 0;
		int n = mp.size();
		if (n == 0) cout << "empty task" << endl;
		else {
			map<int, ListNode*>::iterator it = mp.begin();
			cout << "当前一共有" << n << "个数据: " << endl;
			for (; it != mp.end(); ++it) 
			{
				cout << "key值为:" << it->first << ",value值为: " << it->second->l_value << endl;
				i++;
			}
		}
	}

private:
	int m_capacity;    
	ListNode* pHead;   
	ListNode* pTail;   
	map<int, ListNode*>  mp;  
};

