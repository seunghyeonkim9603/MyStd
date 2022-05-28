#include <memory>
#include <string>
#include <iostream>

#include "ObjectPool.h"

class Player
{
public:
	Player(int x, int y, int id, std::string name)
		: mX(x),
		mY(y),
		mId(id),
		mName(name)
	{
		std::cout << mId << " : " << "constructor" << std::endl;
	}

	~Player()
	{
		std::cout << mId << " : " << "destructor" << std::endl;
	}

	std::string GetName()
	{
		return mName;
	}
	int GetID()
	{
		return mId;
	}

private:
	int mX;
	int mY;
	int mId;
	std::string mName;
};

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