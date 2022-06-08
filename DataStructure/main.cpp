#include <assert.h>
#include <random>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include "RedBlackTree.h"

int main(void)
{
	RedBlackTree<int> tree;
	std::vector<int> v;

	for (int i = 0; i < 50; ++i)
	{
		int random = rand() % 50;

		v.push_back(random);
		tree.Insert(random);
		assert(tree.IsRedBlackTree());
	}
	for (int i = 0; i < 50; ++i)
	{
		int removed = v[i];
		std::cout << "remove : " << removed << std::endl;
		tree.Print();
		if (removed == 5)
		{
			removed = 5;
		}
		assert(tree.Remove(removed));
		assert(tree.IsRedBlackTree());
		_getch();
		system("cls");
	}
}