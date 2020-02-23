#ifndef TREE_H_
#define TREE_H_

#include "node.h"

#include <memory>
#include <functional>
#include <vector>
#include <stack>

template<typename T>
class BinarySearchTree
{
public:
	using RealTType = std::remove_reference_t<std::decay_t<T>>;
	using NodeType = Node<RealTType>;
	using UniqueNodeType = std::unique_ptr<NodeType>;

	BinarySearchTree() :root_(nullptr)
	{
	}

	~BinarySearchTree() = default;

	BinarySearchTree(const BinarySearchTree&) = delete;
	BinarySearchTree& operator=(const BinarySearchTree&) = delete;

	BinarySearchTree(BinarySearchTree&&) = delete;
	BinarySearchTree& operator=(BinarySearchTree&&) = delete;

public:

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
		root_ = Put(std::move(root_), std::forward<NodeValType>(val));
	}

	const int Height()const noexcept
	{
		return Height(root_.get());
	}

	template<typename NodeValType>
	NodeType const*const Floor(NodeValType&& val)const noexcept
	{
		return Floor(root_.get(), std::forward<NodeValType>(val));
	}

	template<typename NodeValType>
	NodeType const*const Ceiling(NodeValType&& val)const noexcept
	{
		return Ceiling(root_.get(), std::forward<NodeValType>(val));
	}

	NodeType const*const Min()const noexcept
	{
		return Min(root_.get());
	}

	NodeType const*const Max()const noexcept
	{
		return Max(root_.get());
	}

	void DelMin()
	{
		root_ = DelMin(std::move(root_));
	}

	void DelMax()
	{
		root_ = DelMax(std::move(root_));
	}

	template<typename NodeValType>
	void Delete(NodeValType&& val)
	{
		root_ = Delete(std::move(root_),std::forward<NodeValType>(val));
	}

	NodeType const*const Select(int ranking)const noexcept
	{
		return Select(root_.get(), ranking);
	}

	template<typename NodeValType>
	const int Rank(NodeValType&& val)const noexcept
	{
		return Rank(root_.get(), std::forward<NodeValType>(val));
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
		MiddleOrderWithRecursion(root_.get(), std::forward<TTraversingCb>(fun));
	}

	template<typename TTraversingCb>
	void PreOrderWithRecursion(TTraversingCb&& fun)const noexcept
	{
		PreOrderWithRecursion(root_.get(), std::forward<TTraversingCb>(fun));
	}

	template<typename TTraversingCb>
	void LastOrderWithRecursion(TTraversingCb&& fun)const noexcept
	{
		LastOrderWithRecursion(root_.get(), std::forward<TTraversingCb>(fun));
	}

	const int Size()const noexcept
	{
		return Size(root_.get());
	}

private:

	template<typename NodeValType>
	UniqueNodeType Put(UniqueNodeType node,NodeValType&& param)
	{
		if (node == nullptr)
		{
			return std::make_unique<NodeType>(std::forward<NodeValType>(param));
		}

		if (param < node->val)
		{
			node->left = Put(std::move(node->left), std::forward<NodeValType>(param));
		}
		else if (param > node->val)
		{
			node->right = Put(std::move(node->right), std::forward<NodeValType>(param));
		}

		node->sub_node_num = Size(node->left.get()) + Size(node->right.get()) + 1;

		return node;
	}

	const int Height(NodeType* node)const noexcept
	{
		if (node == nullptr) 
		{
			return 0;
		}

		int left_height = Height(node->left.get());
		int right_height = Height(node->right.get());

		return 1 + (left_height >= right_height ? left_height : right_height);
	}

	template<typename NodeValType>
	NodeType const*const Floor(NodeType* node, NodeValType&& val)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (val <= node->val)
		{
			return Floor(node->left.get(), std::forward<NodeValType>(val));
		}

		auto tmp = Floor(node->right.get(), std::forward<NodeValType>(val));
		if (tmp == nullptr)
		{
			return node;
		}
		
		return tmp;
	}

	template<typename NodeValType>
	NodeType const*const Ceiling(NodeType* node, NodeValType&& val)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (val >= node->val)
		{
			return Ceiling(node->right.get(), std::forward<NodeValType>(val));
		}

		auto tmp = Ceiling(node->left.get(), std::forward<NodeValType>(val));
		if (tmp == nullptr)
		{
			return node;
		}

		return tmp;
	}

	NodeType* Min(NodeType* node)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		auto min_node = Min(node->left.get());
		if (min_node == nullptr)
		{
			return node;
		}

		return min_node;
	}

	NodeType const*const Max(NodeType* node)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		auto max_node = Max(node->right.get());
		if (max_node == nullptr)
		{
			return node;
		}

		return max_node;
	}

	UniqueNodeType DelMin(UniqueNodeType node)
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (node->left == nullptr)
		{
			return std::move(node->right);
		}

		node->left = DelMin(std::move(node->left));

		node->sub_node_num = Size(node->left.get()) + Size(node->right.get()) + 1;

		return node;
	}

	UniqueNodeType DelMax(UniqueNodeType node)
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (node->right == nullptr)
		{
			return std::move(node->left);
		}

		node->right = DelMax(std::move(node->right));

		node->sub_node_num = Size(node->left.get()) + Size(node->right.get()) + 1;

		return node;
	}

	template<typename NodeValType>
	UniqueNodeType Delete(UniqueNodeType node, NodeValType&& val)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		if (val < node->val)
		{
			node->left = Delete(std::move(node->left), std::forward<NodeValType>(val));
		}
		else if (val > node->val)
		{
			node->right = Delete(std::move(node->right), std::forward<NodeValType>(val));
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
			
			UniqueNodeType tmp = node;
			node.reset(Min(tmp->right.get()));
			node->right = DelMin(std::move(tmp->right));
			node->left = tmp->left;
		}

		node->sub_node_num = Size(node->left.get()) + Size(node->right.get()) + 1;

		return node;
	}

	NodeType const*const Select(NodeType* node,int ranking)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		int num = Size(node->left.get()) + 1;
		if (num > ranking)
		{
			return Select(node->left.get(), ranking);
		}
		else if (num < ranking)
		{
			return Select(node->right.get(), ranking - num);
		}
		
		return node;
	}

	template<typename NodeValType>
	int Rank(NodeType* node,NodeValType&& val)const noexcept
	{
		if (node == nullptr)
		{
			return 0;
		}
		if (val < node->val)
		{
			return Rank(node->left.get(), std::forward<NodeValType>(val));
		}
		else if (val > node->val)
		{
			int rank = Rank(node->right.get(), std::forward<NodeValType>(val));
			if (rank == 0)
			{
				return rank;
			}
			return 1 + Size(node->left.get()) + rank;
		}

		return Size(node->left.get()) + 1;
	}

	template<typename TTraversingCb>
	void MiddleOrderWithRecursion(NodeType* node, TTraversingCb&& fun)const noexcept
	{
		if (nullptr == node)
		{
			return;
		}
		MiddleOrderWithRecursion(node->left.get(), std::forward<TTraversingCb>(fun));
		fun(node->val);
		MiddleOrderWithRecursion(node->right.get(), std::forward<TTraversingCb>(fun));
	}

	template<typename TTraversingCb>
	void MiddleOrderWithStack(NodeType* node, TTraversingCb&& fun)
	{
		if (nullptr == node)
		{
			return;
		}

		std::stack<NodeType*> node_stack;

		while (nullptr != node || !node_stack.empty())
		{
			if (nullptr != node)
			{
				node_stack.push(node);
				node = node->left.get();
			}
			else
			{
				node = node_stack.top();
				fun(node->val);
				node_stack.pop();
				node = node->right.get();
			}
		}
	}

	template<typename TTraversingCb>
	void PreOrderWithRecursion(NodeType* node, TTraversingCb&& fun)const noexcept
	{
		if (nullptr == node)
		{
			return;
		}
		fun(node->val);
		PreOrderWithRecursion(node->left.get(), std::forward<TTraversingCb>(fun));
		PreOrderWithRecursion(node->right.get(), std::forward<TTraversingCb>(fun));
	}

	template<typename TTraversingCb>
	void PreOrderWithStack(NodeType* node, TTraversingCb&& fun)
	{
		if (nullptr == node)
		{
			return;
		}

		std::stack<NodeType*> node_stack;
		while (node != nullptr || !node_stack.empty())
		{
			if (node != nullptr)
			{
				fun(node->val);
				node_stack.push(node);
				node = node->left.get();
			}
			else
			{
				node = node_stack.top();
				node_stack.pop();
				node = node->right.get();
			}
		}
	}

	template<typename TTraversingCb>
	void LastOrderWithRecursion(NodeType* node, TTraversingCb&& fun)const noexcept
	{
		if (nullptr == pRoot)
		{
			return;
		}
		LastOrderWithRecursion(node->left.get(), std::forward<TTraversingCb>(fun));
		LastOrderWithRecursion(node->right.get(), std::forward<TTraversingCb>(fun));
		fun(node->val);
	}

	template<typename TTraversingCb>
	void LastOrderWithStack(NodeType* node, TTraversingCb&& fun)
	{
		if (nullptr == node)
		{
			return;
		}

		std::stack<NodeType*> tmp_stack,out_stack;

		tmp_stack.push(node);
		while (!tmp_stack.empty())
		{
			NodeType* curr = tmp_stack.top();
			out_stack.push(curr);
			tmp_stack.pop();
			if (curr->left.get())
			{
				tmp_stack.push(curr->left.get());
			}
				
			if (curr->right.get())
			{
				tmp_stack.push(curr->right.get());
			}	
		}
		while (!out_stack.empty())
		{
			fun(out_stack.top()->val);
			out_stack.pop();
		}
	}

	const int Size(NodeType* node)const noexcept
	{
		if (node == nullptr)
		{
			return 0;
		}

		return node->sub_node_num;
	}
	
private:
	UniqueNodeType root_;

};

template<>
class BinarySearchTree<float> {};

template<>
class BinarySearchTree<double> {};

#endif // !TREE_H_

