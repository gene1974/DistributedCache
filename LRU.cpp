//单链表能实现新来的放头部，最久不用的在尾部删除。但删除的时候需要遍历到尾部，因为单链表只有头指针。
//在用到已经用到过的数据时，还要遍历整合链表，来确定是否用过，然后再遍历到响应位置来剔除的节点，并重新放在头部。这效率可想而知。
//这时hashmap的作用就出来了 他可以在单位1的时间判断value的值是否存在，key直接存储节点对象，能直接定位删除对应的节点(将比节点的父节点指向子节点)。
//要通过一个节点直接获得父节点的话，通过单链表是不行的。
//这时双向链表的作用也提现出来了。能直接定位到父节点。 这效率就很高了。而且由于双向链表有尾指针，所以剔除最后的尾节点也十分方便，快捷。

#include<iostream>
#include"LRUCache.h"

using namespace std;

int main()
{
	LRU_Cache* lruCache = new LRU_Cache(5);

	lruCache->Insert(5, 2);
	lruCache->Insert(2, 3);
	lruCache->Insert(3, 5);
	lruCache->Insert(6, 4);
	cout << "The size is " << lruCache->GetSize() << endl;
	cout << "if key = 3, the value is " << lruCache->Get(3) << endl;

	lruCache->Insert(4, 2);

	lruCache->show();

	lruCache->Insert(0, 0);

	lruCache->show();

	return 0;

}
//Set()函数可以存储client的数据，按照key-value的形式写入
//需要解决的问题是：
// 1.设置储存的大小，装多少个值就够了
// 2.怎么接口client的数据，怎么把一堆数据通过set()函数全部写入（key的值怎么对应）
// 3.怎么分配给多个cashe server 
// 4.怎么展示储存的数据
// 5.怎么把读到的value由返回Client
// 6.怎么与master挂钩
// 7.多线程如何实现？通过信号量？
// 8.如果随机数据里有-1，怎么解决返回值为-1的情况？