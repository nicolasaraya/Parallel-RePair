#include <iostream>
#include "DList.h"

using namespace std;

int main(int argc, char const *argv[])
{
	DList* d = new DList(); 

	d->insertEnd(1);
	d->insertStart(2);
	d->insertEnd(3);
	Nodo* b = d->back();
	d->insertBefore(b, 10);
	delete(d);
	return 0;
}