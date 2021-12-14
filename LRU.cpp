#include<iostream>
#include"LRUCache.h"

using namespace std;

int main()
{
	LRU_Cache* lruCache = new LRU_Cache(5);

	lruCache->Insert(1, "fdakf");
	lruCache->Insert(2, "adfkja");
	lruCache->Insert(5, "adfa");
	lruCache->Insert(6, "kjhdf");
	cout << "The size is " << lruCache->GetSize() << endl;
	cout << "Now we try to get key = 3: " << lruCache->Get(3) << endl;

	lruCache->Insert(4, "adsfkh");

	lruCache->show();

	lruCache->Insert(0, "dasfkha");

	lruCache->show();

	return 0;

}