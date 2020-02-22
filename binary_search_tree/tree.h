#ifndef TREE_H_
#define TREE_H_

#include "node.h"

#include <memory>
#include <functional>
#include <vector>

template<typename T>
class BinarySearchTree
{
public:
	using RealTType = std::remove_reference_t<std::decay_t<T>>;
	using NodeType = Node<RealTType>;
	using ShareNodeType = std::shared_ptr<NodeType>;

	BinarySearchTree() :root_(nullptr)
	{
	}

	~BinarySearchTree() = default;

	BinarySearchTree(const BinarySearchTree&) = delete;
	BinarySearchTree& operator=(const BinarySearchTree&) = delete;

	BinarySearchTree(BinarySearchTree&&) = delete;
	BinarySearchTree& operator=(BinarySearchTree&&) = delete;

	template<typename NodeValType,
		typename = typename std::enable_if_t<
		std::is_same<
		RealTType,
		std::remove_reference_t<std::decay_t<NodeValType>>
		>::value
	>
	>
	void Put(NodeValType&& val)
	{
		root_ = Put(root_, std::forward<RealTType>(val));
	}

	const int Height()const noexcept
	{
		return Height(root_);
	}

	template<typename NodeValType>
	NodeType const*const Floor(NodeValType&& val)const noexcept
	{
		return Floor(root_, std::forward<NodeValType>(val));
	}

	template<typename NodeValType>
	NodeType const*const Ceiling(NodeValType&& val)const noexcept
	{
		return Ceiling(root_, std::forward<NodeValType>(val));
	}

	NodeType const*const Min()const noexcept
	{
		return Min(root_).get();
	}

	NodeType const*const Max()const noexcept
	{
		return Max(root_).get();
	}

	void DelMin()
	{
		root_ = DelMin(root_);
	}

	void DelMax()
	{
		root_ = DelMax(root_);
	}

	template<typename NodeValType>
	void Delete(NodeValType&& val)
	{
		root_ = Delete(root_,std::forward<NodeValType>(val));
	}

	NodeType const*const Select(int ranking)const noexcept
	{
		return Select(root_, ranking).get();
	}

	template<typename NodeValType>
	const int Rank(NodeValType&& val)const noexcept
	{
		return Rank(root_, std::forward<NodeValType>(val));
	}

	NodeType const*const GetRoot()const noexcept
	{
		if (root_ == nullptr)
		{
			return nullptr;
		}
		return root_.get();
	}

	template<typename TTraversingCb>
	void MiddleOrderWithRecursion(TTraversingCb&& fun)const noexcept
	{
		MiddleOrderWithRecursion(root_, std::forward<TTraversingCb>(fun));
	}

	template<typename TTraversingCb>
	void PreOrderWithRecursion(TTraversingCb&& fun)const noexcept
	{
		PreOrderWithRecursion(root_, std::forward<TTraversingCb>(fun));
	}

	template<typename TTraversingCb>
	void LastOrderWithRecursion(TTraversingCb&& fun)const noexcept
	{
		LastOrderWithRecursion(root_, std::forward<TTraversingCb>(fun));
	}

	const int Size()const noexcept
	{
		return Size(root_);
	}

private:

	template<typename NodeValType>
	ShareNodeType Put(ShareNodeType node,NodeValType&& param)
	{
		if (node == nullptr)
		{
			return std::make_shared<NodeType>(std::forward<RealTType>(param));
		}

		if (param < node->val)
		{
			node->left = Put(node->left, std::forward<RealTType>(param));
		}
		else if (param > node->val)
		{
			node->right = Put(node->right, std::forward<RealTType>(param));
		}

		node->sub_node_num = Size(node->left) + Size(node->right) + 1;

		return node;
	}

	const int Height(ShareNodeType node)const noexcept
	{
		if (node == nullptr) 
		{
			return 0;
		}

		int left_height = Height(node->left);
		int right_height = Height(node->right);

		return 1 + (left_height >= right_height ? left_height : right_height);
	}

	template<typename NodeValType>
	NodeType const*const Floor(ShareNodeType node, NodeValType&& val)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (val <= node->val)
		{
			return Floor(node->left, std::forward<NodeValType>(val));
		}

		auto tmp = Floor(node->right, std::forward<NodeValType>(val));
		if (tmp == nullptr)
		{
			return node.get();
		}
		
		return tmp;
	}

	template<typename NodeValType>
	NodeType const*const Ceiling(ShareNodeType node, NodeValType&& val)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (val >= node->val)
		{
			return Ceiling(node->right, std::forward<NodeValType>(val));
		}

		auto tmp = Ceiling(node->left, std::forward<NodeValType>(val));
		if (tmp == nullptr)
		{
			return node.get();
		}

		return tmp;
	}

	ShareNodeType Min(ShareNodeType node)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		auto min_node = Min(node->left);
		if (min_node == nullptr)
		{
			return node;
		}

		return min_node;
	}

	ShareNodeType Max(ShareNodeType node)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		auto max_node = Max(node->right);
		if (max_node == nullptr)
		{
			return node;
		}

		return max_node;
	}

	ShareNodeType DelMin(ShareNodeType node)
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (node->left == nullptr)
		{
			return node->right;
		}

		node->left = DelMin(node->left);

		node->sub_node_num = Size(node->left) + Size(node->right) + 1;

		return node;
	}

	ShareNodeType DelMax(ShareNodeType node)
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (node->right == nullptr)
		{
			return node->left;
		}

		node->right = DelMax(node->right);

		node->sub_node_num = Size(node->left) + Size(node->right) + 1;

		return node;
	}

	template<typename NodeValType>
	ShareNodeType Delete(ShareNodeType node, NodeValType&& val)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		if (val < node->val)
		{
			node->left = Delete(node->left, std::forward<NodeValType>(val));
		}
		else if (val > node->val)
		{
			node->right = Delete(node->right, std::forward<NodeValType>(val));
		}
		else
		{
			if (node->left == nullptr)
			{
				return node->right;
			}
			if (node->right == nullptr)
			{
				return node->left;
			}
			
			ShareNodeType tmp = node;
			node = Min(tmp->right);
			node->right = DelMin(tmp->right);
			node->left = tmp->left;
		}

		node->sub_node_num = Size(node->left) + Size(node->right) + 1;

		return node;
	}

	ShareNodeType Select(ShareNodeType node,int ranking)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		int num = Size(node->left) + 1;
		if (num > ranking)
		{
			return Select(node->left, ranking);
		}
		else if (num < ranking)
		{
			return Select(node->right, ranking - num);
		}
		
		return node;
	}

	template<typename NodeValType>
	int Rank(ShareNodeType node,NodeValType&& val)const noexcept
	{
		if (node == nullptr)
		{
			return 0;
		}
		if (val < node->val)
		{
			return Rank(node->left, std::forward<NodeValType>(val));
		}
		else if (val > node->val)
		{
			int rank = Rank(node->right, std::forward<NodeValType>(val));
			if (rank == 0)
			{
				return rank;
			}
			return 1 + Size(node->left) + rank;
		}

		return Size(node->left) + 1;
	}

	template<typename TTraversingCb>
	void MiddleOrderWithRecursion(ShareNodeType node, TTraversingCb&& fun)const noexcept
	{
		if (nullptr == node)
		{
			return;
		}
		MiddleOrderWithRecursion(node->left, std::forward<TTraversingCb>(fun));
		fun(node->val);
		MiddleOrderWithRecursion(node->right, std::forward<TTraversingCb>(fun));
	}

	template<typename TTraversingCb>
	void PreOrderWithRecursion(ShareNodeType node, TTraversingCb&& fun)const noexcept
	{
		if (nullptr == node)
		{
			return;
		}
		fun(node->val);
		PreOrderWithRecursion(node->left, std::forward<TTraversingCb>(fun));
		PreOrderWithRecursion(node->right, std::forward<TTraversingCb>(fun));
	}

	template<typename TTraversingCb>
	void LastOrderWithRecursion(ShareNodeType node, TTraversingCb&& fun)const noexcept
	{
		if (nullptr == pRoot)
		{
			return;
		}
		LastOrderWithRecursion(node->left, std::forward<TTraversingCb>(fun));
		LastOrderWithRecursion(node->right, std::forward<TTraversingCb>(fun));
		fun(node->val);
	}

	const int Size(ShareNodeType node)const noexcept
	{
		if (node == nullptr)
		{
			return 0;
		}

		return node->sub_node_num;
	}
	
private:
	ShareNodeType root_;

};

template<>
class BinarySearchTree<float> {};

template<>
class BinarySearchTree<double> {};

#endif // !TREE_H_

