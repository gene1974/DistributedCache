#include "LRUCache.h"

#include <iostream>
#include <map>  
#include <algorithm>

ListNode::ListNode(int key, string value) : l_key(key), l_value(value), Pre(NULL), Next(NULL) {};

LRU_Cache::LRU_Cache(int size) : m_capacity(size), pHead(nullptr), pTail(nullptr) {}

LRU_Cache::~LRU_Cache() {
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

void LRU_Cache::Remove(ListNode* pNode) {
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

void LRU_Cache::SetHead(ListNode* pNode)
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

void LRU_Cache::Insert(int key, string value)
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
            //cout << endl << "警告：已经超出最大容量:" << m_capacity << "个数据" << endl;
            int x = pTail->l_key;
            map<int, ListNode*>::iterator it = mp.find(x);
            Remove(pTail); //移除表尾指针内存
            string y = it->second->l_value;
            delete it->second;//删除value
            //cout << endl << "为您移除key为" << x << ",value为" << y << "的节点" << endl;
            mp.erase(it);
        }
        SetHead(NewNode);//放到头部
        mp[key] = NewNode;
    }
}

string LRU_Cache::Get(int key)
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
        return "Sorry, we don't find it";    
    }
}

int LRU_Cache::GetSize()
{
    return mp.size();
}

void LRU_Cache::show() {
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

void LRU_Cache::reset_size(int size_num)
{
    if (size_num > m_capacity) {
        cout << "执行扩容操作：" << endl;
        m_capacity = size_num;
        cout << "容量大小变为" << m_capacity <<endl;
    }
    else if(size_num < m_capacity)
    {
        int n = m_capacity - size_num;
        while (n > 0)
        {
            int x = pTail->l_key;
            map<int, ListNode*>::iterator it = mp.find(x);
            Remove(pTail); //移除表尾指针内存
            string y = it->second->l_value;
            delete it->second;//删除value
            m_capacity--;
            cout << endl << "为您移除key为" << x << ",value为" << y << "的节点" << "并缩容到" << m_capacity << endl;
            mp.erase(it);
            n--;
        }
        
    }
    else {
        cout << "容量大小未发生变化" << endl;
    }
}
