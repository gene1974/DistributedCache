#include "LRUCache.h"

#include <iostream>
#include <map>  
#include <algorithm>

LRU_Cache::LRU_Cache(int size) : m_capacity(size), pHead(nullptr), pTail(nullptr) {}

LRU_Cache::~LRU_Cache() {
    std::map<std::string, ListNode*>::iterator it = mp.begin();
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

void LRU_Cache::Insert(std::string key, int value)
{
    std::map<std::string, ListNode*>::iterator it = mp.find(key);//如果新key等于旧key，则更新value
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
            std::cout << std::endl << "[Cache] "  << "警告：已经超出最大容量:" << m_capacity << "个数据" << std::endl;
            std::string x = pTail->l_key;
            std::map<std::string, ListNode*>::iterator it = mp.find(x);
            Remove(pTail); //移除表尾指针内存
            int y = it->second->l_value;
            delete it->second;//删除value
            std::cout << "[Cache] "  << "为您移除key为" << x << ",value为" << y << "的节点" << std::endl << std::endl;
            mp.erase(it);
        }
        SetHead(NewNode);//放到头部
        mp[key] = NewNode;
    }
}

int LRU_Cache::Get(std::string key)//调用get就会更新数据，转移至开头
{
    std::map<std::string, ListNode*>::iterator it = mp.find(key);
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

int LRU_Cache::GetSize()
{
    return mp.size();
}

void LRU_Cache::show() {
    int i = 0;
    int n = mp.size();
    if (n == 0) std::cout << "empty task" << std::endl;
    else {
        std::map<std::string, ListNode*>::iterator it = mp.begin();
        std::cout << "[Cache] "  << "当前一共有" << n << "个数据: " << std::endl;
        for (; it != mp.end(); ++it)
        {
            std::cout << "key值为:" << it->first << ",value值为: " << it->second->l_value << std::endl;
            i++;
        }
        std::cout << std::endl;
    }
}

void LRU_Cache::reset_size(int size_num)
{
    if (size_num > m_capacity) {
        std::cout << "[Cache] "  << "执行扩容操作：" << std::endl;
        m_capacity = size_num;
        std::cout << "[Cache] "  << "容量大小变为" << m_capacity << std::endl;
    }
    else if (size_num < m_capacity)
    {
        int n = m_capacity - size_num;
        while (n > 0)
        {
            std::string x = pTail->l_key;
            std::map<std::string, ListNode*>::iterator it = mp.find(x);
            Remove(pTail); //移除表尾指针内存
            int y = it->second->l_value;
            delete it->second;//删除value
            m_capacity--;
            std::cout << std::endl << "为您移除key为" << x << ",value为" << y << "的节点" << "并缩容到" << m_capacity << std::endl;
            mp.erase(it);
            n--;
        }

    }
    else {
        std::cout << "容量大小未发生变化" << std::endl;
    }
}
