#include <Windows.h>
#include <memory>
#include <string>
#include <iostream>

#include "ObjectPool.h"



int main(void)
{
	ObjectPool<Player> pool(30);

	for (int i = 0; i < 30; ++i)
	{
		Player* p = pool.GetObject(0, 0, i, "aa");

		std::cout << p->GetID() << std::endl;

		pool.ReleaseObject(p);
	}
}