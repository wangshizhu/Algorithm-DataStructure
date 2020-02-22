#include "tree.h"

#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <numeric>
#include <chrono>
#include <ctime>
#include <random>

void PrintFormat(const char* p)
{
	std::cout << std::endl;
	std::cout << "----------"<<p<<"----------" << std::endl;
}

void TestPutInt(BinarySearchTree<int>& bst)
{
	PrintFormat("TestPutInt");

	std::random_device r;

	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(1, 1000);

	for (int i=0;i<100;i++)
	{
		bst.Put(uniform_dist(e1));
	}

	std::cout << "size:" << bst.Size() << std::endl;
}

void TestFloor(const BinarySearchTree<int>& bst)
{
	if (bst.GetRoot() == nullptr)
	{
		return;
	}

	PrintFormat("TestFloor");

	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(1, 1000);
	
	int finder = uniform_dist(e1);
	std::cout << "val of root:" << bst.GetRoot()->val << std::endl;
	std::cout << "floor finder:" << finder << std::endl;
	
	auto node = bst.Floor(finder);
	if (node  == nullptr)
	{
		std::cout << "dont find" << std::endl;
		return;
	}
	std::cout << "floor:" << node->val<< std::endl;
}

void TestCeiling(const BinarySearchTree<int>& bst)
{
	if (bst.GetRoot() == nullptr)
	{
		return;
	}

	PrintFormat("TestCeiling");

	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(1, 1000);

	int finder = uniform_dist(e1);
	std::cout << "val of root:" << bst.GetRoot()->val << std::endl;
	std::cout << "ceiling finder:" << finder << std::endl;

	auto node = bst.Ceiling(finder);
	if (node == nullptr)
	{
		std::cout << "dont find" << std::endl;
		return;
	}
	std::cout << "ceiling:" << node->val << std::endl;
}

void TestMin(const BinarySearchTree<int>& bst)
{
	if (bst.GetRoot() == nullptr)
	{
		return;
	}

	PrintFormat("TestMin");

	auto node = bst.Min();
	std::cout <<"min val:"<< node->val << std::endl;
}

void TestMax(const BinarySearchTree<int>& bst)
{
	if (bst.GetRoot() == nullptr)
	{
		return;
	}
	PrintFormat("TestMax");

	auto node = bst.Max();
	std::cout << "max val:" << node->val << std::endl;
}

void TestRankAndSelect(const BinarySearchTree<int>& bst)
{
	PrintFormat("TestRankAndSelect");

	int ranking = 0;
	int test_val = 0;
	bst.MiddleOrderWithRecursion([&ranking, &test_val](const auto& val)
	{
		++ranking;
		if (ranking == 20)
		{
			test_val = val;
		}
		std::cout << "ranking:" << ranking << " val:" << val << std::endl;
	});

	std::cout <<"rank of val of test:"<< test_val <<" rank:"<< bst.Rank(test_val) << std::endl;
	std::cout << "real val:" << test_val << " test val:" << bst.Select(20)->val << std::endl;
}

void TestHeight(const BinarySearchTree<int>& bst)
{
	PrintFormat("TestHeight");

	std::cout << "height of BST:" << bst.Height() << std::endl;
}

class Player
{
public:

	explicit Player(int fight_val):fight_val_(fight_val)
	{
		auto now = std::chrono::system_clock::now();
		auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
		
		update_time_ = duration_in_ms.count();
	}

	const int FightVal() const noexcept
	{
		return fight_val_;
	}

	const std::time_t UpdateTime() const noexcept
	{
		return update_time_;
	}

	bool operator<(const Player& dst)
	{
		if (FightVal() < dst.FightVal())
		{
			return true;
		}
		else if (FightVal() > dst.FightVal())
		{
			return false;
		}

		if (UpdateTime() < dst.UpdateTime())
		{
			return true;
		}
		else if (UpdateTime() > dst.UpdateTime())
		{
			return false;
		}

		return false;
	}
	
private:
	int fight_val_;
	std::time_t update_time_;
};

int main()
{
	{
		BinarySearchTree<int> bst;

		TestPutInt(bst);
		TestFloor(bst);
		TestCeiling(bst);
		TestMin(bst);
		TestMax(bst);
		bst.DelMin();
		bst.DelMax();
		TestRankAndSelect(bst);
		TestHeight(bst);
	}

	{

	}

	system("pause");
    return 0;
}


