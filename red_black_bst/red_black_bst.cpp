// red_black_bst_tree.cpp : 定义控制台应用程序的入口点。
//

#include "tree.h"

#include "node.h"

#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <numeric>
#include <chrono>
#include <ctime>
#include <random>

int g_id = 1;

void PrintFormat(const char* p)
{
	std::cout << std::endl;
	std::cout << "----------" << p << "----------" << std::endl;
}

template<typename T>
void TestPutInt(RedBlackBST<T>& bst,int num)
{
	PrintFormat("TestPutInt");

	std::random_device r;

	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(1, 1000);

	for (int i = 0; i < num; i++)
	{
		bst.Put(T(uniform_dist(e1)));
	}

	std::cout << "size:" << bst.Size() << std::endl;
}

template<typename T>
void TestFloor(const RedBlackBST<T>& bst)
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
	if (node == nullptr)
	{
		std::cout << "dont find" << std::endl;
		return;
	}
	std::cout << "floor:" << node->val << std::endl;
}

template<typename T>
void TestCeiling(const RedBlackBST<T>& bst)
{
	if (bst.GetRoot() == nullptr)
	{
		return;
	}

	PrintFormat("TestCeiling");

	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(1, 1000);

	T finder(uniform_dist(e1));
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
const Node<T>* TestMin(const RedBlackBST<T>& bst)
{
	if (bst.GetRoot() == nullptr)
	{
		return nullptr;
	}

	PrintFormat("TestMin");

	auto node = bst.Min();
	std::cout << "min val:" << node->val << std::endl;

	return node;
}

template<typename T>
void TestMax(const RedBlackBST<T>& bst)
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
T TestRankAndSelect(const RedBlackBST<T>& bst,int rank)
{
	PrintFormat("TestRankAndSelect");

	int ranking = 0;
	T test_val(0);
	bst.MiddleOrderWithRecursion([&ranking,&rank, &test_val](const auto& val)
	{
		++ranking;
		if (ranking == rank)
		{
			test_val = val;
		}
		//std::cout << "ranking:" << ranking << " val:" << val << std::endl;
	});

	std::cout << "rank of val of test:" << test_val << " rank:" << bst.Rank(test_val) << std::endl;
	auto node = bst.Select(rank);
	if (node != nullptr)
	{
		std::cout << "real val:" << test_val << " test val:" << node->val << std::endl;
	}

	return test_val;
}

template<typename T>
void TestHeight(const RedBlackBST<T>& bst)
{
	PrintFormat("TestHeight");

	std::cout << "height of BST:" << bst.Height() << std::endl;
}

template<typename T>
void TestGet(const RedBlackBST<T>& bst)
{
	PrintFormat("TestGet");

	auto node = bst.Get(T(10));
	if (node != nullptr)
	{
		std::cout << "The Target Val is existed" << std::endl;
		return;
	}

	std::cout << "The Target Val isnt existed" << std::endl;
}

class Player
{
public:

	explicit Player(int fight_val) :fight_val_(fight_val)
	{
		player_id_ = g_id++;
		auto now = std::chrono::system_clock::now();
		auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

		update_time_ = duration_in_ms.count();
	}

	Player(const Player& p) = default;
	Player& operator=(const Player& p)
	{
		this->player_id_ = p.PlayerId();
		this->update_time_ = p.UpdateTime();
		this->fight_val_ = p.FightVal();

		return *this;
	}

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

	bool SetFightVal(int val)
	{
		if (val == FightVal())
		{
			return false;
		}

		fight_val_ = val;

		return true;
	}

	const std::time_t UpdateTime() const noexcept
	{
		return update_time_;
	}

	const int PlayerId()const noexcept
	{
		return player_id_;
	}

	friend std::ostream & operator <<(std::ostream & os, const Player& dst)
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
			return false;
		}
		else if (UpdateTime() > dst.UpdateTime())
		{
			return true;
		}

		return false;
	}

	bool operator==(const Player& dst)const 
	{
		if (FightVal() == dst.FightVal() && PlayerId() == dst.PlayerId())
		{
			return true;
		}
		return false;
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

	bool operator >=(const Player& dst)const
	{
		if (FightVal() > dst.FightVal())
		{
			return true;
		}
		else if (FightVal() == dst.FightVal())
		{
			if (UpdateTime() < dst.UpdateTime())
			{
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}

private:
	int player_id_;
	int fight_val_;
	std::time_t update_time_;
};

void UpatePlayerFightVal(RedBlackBST<Player>& bst,Player p,int val)
{
	// 先删除
	bst.Delete(p);

	// 对临时变量设置新值
	p.SetFightVal(val);

	// 再添加
	bst.Put(std::move(p));
}

int main()
{
	{
		RedBlackBST<Player> bst;
		TestPutInt(bst, 100000);

		auto del_val = TestRankAndSelect(bst, 20000);
		int new_val = del_val.FightVal() + 200;

		auto now = std::chrono::system_clock::now();
		auto begin = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());

		UpatePlayerFightVal(bst, del_val, new_val);

		now = std::chrono::system_clock::now();
		auto end = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
		auto spend = end - begin;

		std::cout << spend.count() << std::endl;

		TestFloor(bst);
		TestCeiling(bst);
		TestHeight(bst);
		if (bst.IsBST())
		{
			std::cout << "is bst" << std::endl;
		}
		else
		{
			std::cout << "isnt bst" << std::endl;
		}
		if (bst.IsBalanced())
		{
			std::cout << "balanced" << std::endl;
		}
		else
		{
			std::cout << "no balanced" << std::endl;
		}
		if (bst.Is23Tree())
		{
			std::cout << "23Tree" << std::endl;
		}
		else
		{
			std::cout << "no 23Tree" << std::endl;
		}
		if (bst.IsSizeConsistent())
		{
			std::cout << "size correct" << std::endl;
		}
		else
		{
			std::cout << "no size correct" << std::endl;
		}
	}
	/*{
		RedBlackBST<Player> bst;
		TestPutInt(bst,100);
		TestGet(bst);
		TestFloor(bst);
		TestCeiling(bst);
		TestMin(bst);
		TestMax(bst);
		bst.DelMin();
		bst.DelMax();
		auto del_val = TestRankAndSelect(bst,20);
		bst.Delete(del_val);
		TestHeight(bst);
		if (bst.IsBST())
		{
			std::cout << "is bst" << std::endl;
		}
		else
		{
			std::cout << "isnt bst" << std::endl;
		}
		if (bst.IsBalanced())
		{
			std::cout << "balanced" << std::endl;
		}
		else
		{
			std::cout << "no balanced" << std::endl;
		}
		if (bst.Is23Tree())
		{
			std::cout << "23Tree" << std::endl;
		}
		else
		{
			std::cout << "no 23Tree" << std::endl;
		}
		if (bst.IsSizeConsistent())
		{
			std::cout << "size correct" << std::endl;
		}
		else
		{
			std::cout << "no size correct" << std::endl;
		}
	}*/

	system("pause");
    return 0;
}

