using namespace std;

template<typename T>

class Node {
public:
    /*节点存储的数据*/
    T value;
    T address;
    /*指向下一个节点的指针*/
    Node* next;

public:
    /*空构造器*/
    Node() {}

    /*构造器*/
    Node(T a ,T b) {
        value = a;
        address = b;
    }
};

template<typename T>
class SingleLinkedList {
private:
    /*链表长度，存储的结点个数*/
    int size;

    /*表头节点，节点中存储的数据的类型为T*/
    Node<T>* head;

    /*获取链表中指定索引上的节点，仅供内部调用*/
    Node<T>* node(int index);

public:
    /*构造器*/
    SingleLinkedList();

    /*析构函数*/
    ~SingleLinkedList();

    /*获取链表长度，即当前链表中结点的个数*/
    int length();

    /*检查索引范围*/
    bool checkIndex(int index);

    /*添加指定的数据到链表尾部*/
    bool append(T value,T address);

    /*添加指定的数据到指定的索引位置*/
    bool insert(int index, T value, T address);

    /*删除指定位置上的数据*/
    bool remove(int index);

    /*获取指定索引上的数据*/
    T get(int index);

    /*获取指定数据的索引，不存在时返回0*/
    int getIndex(T value, T address);

    /*将指定位置的数据修改为给定的数据element*/
    void set(int index, T value, T address);

    /*打印整个链表*/
    void print();

    /*释放整个链表*/
    void freeList();
};