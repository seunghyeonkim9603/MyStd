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

	std::string input;

	srand(std::time(NULL));

	for (int i = 0; i < 50; ++i)
	{
		system("cls");

		int random = rand() % 100;
		std::cout << "add : " << random << std::endl;

		v.push_back(random);
		tree.Insert(random);

		tree.Print();

		_getch();
		assert(tree.IsRedBlackTree());
	}

	for (int i = 0; i < 50; ++i)
	{
		system("cls");

		int removed = v[i];
		std::cout << "remove : " << removed << std::endl;

		tree.Print();
		assert(tree.Remove(removed));
		assert(tree.IsRedBlackTree());

		_getch();
	}
}