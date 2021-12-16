#include<iostream>
#include"LRUCache.h"

using namespace std;

int main()
{
	LRU_Cache* lruCache = new LRU_Cache(5);
	lruCache->Insert("0", 0);
	lruCache->Insert("bv", 2);
	lruCache->Insert("cd", 3);
	lruCache->Insert("ef", 5);
	lruCache->Insert("gh", 4);
	cout << "The size is " << lruCache->GetSize() << endl;
	cout << "if key = ab, the value is " << lruCache->Get("ab") << endl;

	lruCache->Insert("yu", 1);

	lruCache->show();

	lruCache->reset_size(7);

	lruCache->Insert("sa", 0);

	lruCache->show();

	return 0;

}
