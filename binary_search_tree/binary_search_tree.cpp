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

template<typename T>
void TestPutInt(BinarySearchTree<T>& bst)
{
	PrintFormat("TestPutInt");

	std::random_device r;

	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(1, 1000);

	for (int i=0;i<100;i++)
	{
		bst.Put(T(uniform_dist(e1)));
	}

	std::cout << "size:" << bst.Size() << std::endl;
}

template<typename T>
void TestFloor(const BinarySearchTree<T>& bst)
{
	if (bst.GetRoot() == nullptr)
	{
		return;
	}

	PrintFormat("TestFloor");

	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(1, 1000);
	
	T finder(uniform_dist(e1));
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

template<typename T>
void TestCeiling(const BinarySearchTree<T>& bst)
{
	if (bst.GetRoot() == nullptr)
	{
		return;
	}

	PrintFormat("TestCeiling");

	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(1, 1000);

	T finder( uniform_dist(e1));
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

template<typename T>
void TestMin(const BinarySearchTree<T>& bst)
{
	if (bst.GetRoot() == nullptr)
	{
		return;
	}

	PrintFormat("TestMin");

	auto node = bst.Min();
	std::cout <<"min val:"<< node->val << std::endl;
}

template<typename T>
void TestMax(const BinarySearchTree<T>& bst)
{
	if (bst.GetRoot() == nullptr)
	{
		return;
	}
	PrintFormat("TestMax");

	auto node = bst.Max();
	std::cout << "max val:" << node->val << std::endl;
}

template<typename T>
void TestRankAndSelect(const BinarySearchTree<T>& bst)
{
	PrintFormat("TestRankAndSelect");

	int ranking = 0;
	T test_val(0);
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

template<typename T>
void TestHeight(const BinarySearchTree<T>& bst)
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

	Player(const Player& p) = default;
	Player& operator=(const Player& p) = default;

	Player(Player&& p) = default;
	Player& operator=(Player&& p) = default;

	~Player()
	{
		//std::cout << "~Player() fight_val_:"<< fight_val_ << std::endl;
	}

	const int FightVal() const noexcept
	{
		return fight_val_;
	}

	const std::time_t UpdateTime() const noexcept
	{
		return update_time_;
	}

	friend std::ostream & operator <<(std::ostream & os,const Player& dst)
	{
		os << dst.fight_val_;

		return os;
	}

	bool operator<(const Player& dst)const
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

	bool operator>(const Player& dst)const
	{
		if (FightVal() > dst.FightVal())
		{
			return true;
		}
		else if (FightVal() < dst.FightVal())
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

	bool operator >=(const Player& dst)const 
	{
		if (FightVal() >= dst.FightVal())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator <=(const Player& dst)const
	{
		if (FightVal() <= dst.FightVal())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
private:
	int fight_val_;
	std::time_t update_time_;
};

int main()
{
	{
		BinarySearchTree<Player> bst;

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

	system("pause");
    return 0;
}


