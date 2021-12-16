#include "SingleLinkedList.h"

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
    while (temp->next != NULL) {//从头节点开始，找到最后一个节点
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
    else {
        Node<T>* nIndex_pre = node(index - 1);
        Node<T>* nIndex = node(index);
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
        return;
    }
    //链表不为空
    Node<T>* n = this->head;
    while (NULL != n) {
        cout << n->value << "--"<<n->address;
        cout << endl;
        n = n->next;
    }
    cout << '\n';
}
template<typename T>
T SingleLinkedList<T>::get(int index) {
    return node(index)->address;
}
template<typename T>
bool SingleLinkedList<T>::remove(int index) {
    checkIndex(index);

    if (index == 0) {
        Node<T>* nHead = this->head;
        Node<T>* nHead_next = nHead->next;

        this->head = nHead_next;
    }
    else if (index == size) {
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
