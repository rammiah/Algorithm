#include "Head.h"

int main(void)
{
	Node head(10);
	head.Insert(2);
	head.Insert(12);
	head.Insert(20);
	head.Insert(17);
	head.Insert(3);
	head.Insert(15);
	head.Insert(5);
	head.Check();
	return 0;
}