#include <iostream>
#include <string>  
#include <random>
#include <sstream> 
#include "SingleLinkedList.h"

using namespace std;
using std::string;
using std::random_device;
using std::default_random_engine;

string strRand(int length = 8) {			// length: 产生字符串的长度
    char tmp;							// tmp: 暂存一个随机数
    string buffer;						// buffer: 保存返回值

   
    random_device rd;					// 产生一个 std::random_device 对象 rd
    default_random_engine random(rd());	// 用 rd 初始化一个随机数发生器 random

    for (int i = 0; i < length; i++) {
        tmp = random() % 36;	// 随机一个小于 36 的整数，0-9、A-Z 共 36 种字符
        if (tmp < 10) {			// 如果随机数小于 10，变换成一个阿拉伯数字的 ASCII
            tmp += '0';
        }
        else {				// 否则，变换成一个大写字母的 ASCII
            tmp -= 10;
            tmp += 'A';
        }
        buffer += tmp;
    }
    return buffer;
}
int keyRand() {

    constexpr int MIN = 10000000;
    constexpr int MAX = 99999999;
    std::random_device rd;  // 将用于为随机数引擎获得种子
    std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
    uniform_int_distribution <int> dis(MIN, MAX);
    // 用 dis 变换 gen 所生成的随机 unsigned int 到 [MIN, MAX] 中的 int
    return dis(gen);
}
string *gendata(int n = 1) {
    string* datalist= new string[n]();
    for (int i = 0; i < n; ++i) {
        datalist[i] = std::to_string(keyRand()) + strRand();
     
    }
    return datalist;

}
string writevalue(int n) { 
    string* b;
    b = gendata(n);
        for (int i = 0; i < n; i++)
        {
            *(b + i) = 'w' + *(b + i);
           
            cout  << *(b + i) << endl;
        }
        return *b;
     }

    

template<typename T>
SingleLinkedList<T>::SingleLinkedList() {
    this->head = new Node<T>();//调用默认构造器
    this->size = 0;//初始化长度为0
    this->head->next = NULL;
}

template<typename T>
SingleLinkedList<T>::~SingleLinkedList() {

}

template<typename T>
int SingleLinkedList<T>::length() {
    return this->size;
}


template<typename T>
bool SingleLinkedList<T>::checkIndex(int index) {
    return index >= 0 && index <= size;
}


template<typename T>
Node<T> * SingleLinkedList<T>::node(int index) {
    checkIndex(index);
    Node<T>* n = this->head;
    for (int i = 0; i < index; i++) {
        n = n->next;
    }
    return n;
}


template<typename T>
bool SingleLinkedList<T>::append(T value , T address) {
    Node<T>* newNode = new Node<T>( value,  address);//构造节点对象

    
    if (this->size == 0) {
        this->head = newNode;
    }
       
    Node<T>* temp = this->head;
    while (temp->next != NULL) {//从头节点开始，找到最后一个节点，判断条件中必须是n->next，而不能是n
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->next = NULL;

    size++;

    return true;
}


template<typename T>
bool SingleLinkedList<T>::insert(int index, T value, T address) {
    checkIndex(index);

    Node<T>* newNode = new Node<T>( value, address);

    if (index == 0) {//插入新节点作为链表头节点
        newNode->next = this->head;
        this->head = newNode;
    }
    else if (index == size) {//插入新节点作为尾部节点
        append(value, address);
    }
    else {//插入位置新节点到index索引之上
        //获取index索引上的节点
        Node<T>* nIndex_pre = node(index - 1);//获取index-1位置上的节点
        Node<T>* nIndex = node(index);//获取index索引上的节点
        nIndex_pre->next = newNode;
        newNode->next = nIndex;
    }
    size++;
    return true;
}


template<typename T>
void SingleLinkedList<T>::print() {
    //如果链表为空
    if (NULL == this->head) {
        cout << "This list is empty.";
        return;//结果方法运行，下面的代码不再执行
    }
    //链表不为空
    Node<T>* n = this->head;
    while (NULL != n) {//此处的判断条件中，只能是n，而不能用n->next
        cout << n->value << "--"<<n->address;
        cout << endl;
        n = n->next;
    }
    cout << '\n';//换行
}
template<typename T>
T SingleLinkedList<T>::get(int index) {
    return node(index)->address;
}
template<typename T>
bool SingleLinkedList<T>::remove(int index) {
    checkIndex(index);

    if (index == 0) {//删除是头节点
        Node<T>* nHead = this->head;
        Node<T>* nHead_next = nHead->next;

        this->head = nHead_next;
    }
    else if (index == size) {//删除的是链表最后一个节点
        Node<T>* nIndex_pre = node(index - 1);
        nIndex_pre->next = NULL;
    }
    else {//删除的是中间节点
        Node<T>* nPre = node(index - 1);
        Node<T>* nNext = node(index + 1);
        nPre->next = nNext;
    }
    size--;
    return true;
}
template<typename T>
void SingleLinkedList<T>::freeList() {

    Node<T>* n1 = this->head, * n2;
    while (NULL != n1) {
        n2 = n1->next;
        delete n1;
        n1 = n2;
    }
}





    int main(){
        
        SingleLinkedList<string> list;
        list.append(*gendata(1), "1");
        list.append(*gendata(1), "1");
        list.append(*gendata(1), "2");
        list.append(*gendata(1), "3");
        list.append(*gendata(1), "0");
        list.print();
        cout << list.length() << endl;
       

        return 0;

    }