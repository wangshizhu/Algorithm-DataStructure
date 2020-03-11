#ifndef TREE_H_
#define TREE_H_

#include "node.h"

#include <memory>

template<typename T>
class RedBlackBST
{
public:
	using RealTType = std::remove_reference_t<std::decay_t<T>>;
	using NodeType = Node<RealTType>;
	using UniqueNodeType = std::unique_ptr<NodeType>;

	RedBlackBST() :root_(nullptr)
	{
	}

	~RedBlackBST() = default;

	RedBlackBST(const RedBlackBST&) = delete;
	RedBlackBST& operator=(const RedBlackBST&) = delete;

	RedBlackBST(RedBlackBST&&) = delete;
	RedBlackBST& operator=(RedBlackBST&&) = delete;

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
		root_->color = NodeType::BLACK;
	}

	template<typename NodeValType>
	NodeType const*const Get(NodeValType&& val) const noexcept
	{
		return Get(root_.get(), std::forward<NodeValType>(val));
	}

	const int Height()const noexcept
	{
		return Height(root_.get());
	}

	template<typename NodeValType>
	const bool IsExists(NodeValType&& val) const noexcept
	{
		return Get(std::forward<NodeValType>(val)) != nullptr;
	}

	const bool IsBST()const noexcept
	{
		return IsBST(root_.get(),nullptr,nullptr);
	}

	const bool Is23Tree()const noexcept
	{
		return Is23Tree(root_.get());
	}

	const bool IsBalanced()const noexcept
	{
		int black = 0;
		NodeType* tmp = root_.get();
		while (tmp != nullptr)
		{
			if (!IsRed(tmp))
			{
				black++;
			}
			tmp = tmp->left.get();
		}
		return IsBalanced(root_.get(),black);
	}

	const bool IsSizeConsistent()const noexcept
	{
		return IsSizeConsistent(root_.get());
	}

	void DelMin()
	{
		if (IsEmpty())
		{
			return;
		}

		if (!IsRed(root_->left.get()) && !IsRed(root_->right.get()))
		{
			root_->color = NodeType::RED;
		}

		root_ = DelMin(std::move(root_));
		if (!IsEmpty())
		{
			root_->color = NodeType::BLACK;
		}
	}

	void DelMax()
	{
		if (IsEmpty())
		{
			return;
		}
		if (!IsRed(root_->left.get())&& !IsRed(root_->right.get()))
		{
			root_->color = NodeType::RED;
		}

		root_ = DelMax(std::move(root_));

		if (!IsEmpty())
		{
			root_->color = NodeType::BLACK;
		}
	}

	NodeType const*const Min()const noexcept
	{
		return Min(root_.get());
	}

	NodeType const*const Max()const noexcept
	{
		return Max(root_.get());
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

	template<typename NodeValType>
	void Delete(NodeValType&& val)
	{
		if (!IsExists(std::forward<NodeValType>(val)))
		{
			return;
		}

		if (!IsRed(root_->left.get()) && !IsRed(root_->right.get()))
		{
			root_->color = NodeType::RED;
		}

		root_ = Delete(std::move(root_), std::forward<NodeValType>(val));

		if (!IsEmpty())
		{
			root_->color = NodeType::BLACK;
		}
	}

	const bool IsEmpty()const noexcept
	{
		return root_ == nullptr;
	}

	const int Size()const noexcept
	{
		return Size(root_.get());
	}

	NodeType const*const GetRoot()const noexcept
	{
		if (root_ == nullptr)
		{
			return nullptr;
		}
		return root_.get();
	}

private:

	template<typename NodeValType>
	UniqueNodeType Put(UniqueNodeType node, NodeValType&& param)
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

		// 右子节点是红节点，左子节点是黑节点，要左旋转
		if (IsRed(node->right.get()) && !IsRed(node->left.get()))
		{
			node = RotateLeft(std::move(node));
		}

		// 左子节点是红节点同时左子节点的左子节点也是红节点，要右旋转
		if (IsRed(node->left.get()) && IsRed(node->left->left.get()))
		{
			node = RotateRight(std::move(node));
		}

		// 左子节点和右子节点都是红节点，要变换颜色
		if (IsRed(node->left.get()) && IsRed(node->right.get()))
		{
			FlipColor(node.get());
		}

		node->sub_node_num = Size(node->left.get()) + Size(node->right.get()) + 1;

		return node;
	}

	template<typename NodeValType>
	NodeType const*const Get(NodeType* node, NodeValType&& val)const noexcept
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		if (val < node->val)
		{
			return Get(node->left.get(), std::forward<NodeValType>(val));
		}
		else if (val > node->val)
		{
			return Get(node->right.get(), std::forward<NodeValType>(val));
		}
		else
		{
			return node;
		}
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

	const bool IsBST(NodeType* node, RealTType* min_val, RealTType* max_val)const noexcept
	{
		if (node == nullptr)
		{
			return true;
		}
		if (min_val != nullptr && node->val <= (*min_val))
		{
			return false;
		}
		if (max_val != nullptr && node->val >= (*max_val))
		{
			return false;
		}

		return IsBST(node->left.get(), min_val, &node->val) && IsBST(node->right.get(), &node->val, max_val);
	}

	const bool Is23Tree(NodeType* node) const noexcept
	{
		if (node == nullptr)
		{
			return true;
		}
		if (IsRed(node->right.get()))
		{
			return false;
		}
		if (node != root_.get() && IsRed(node) && IsRed(node->left.get()))
		{
			return false;
		}

		return Is23Tree(node->left.get()) && Is23Tree(node->right.get());
	}

	const bool IsBalanced(NodeType* node,int black) const noexcept
	{
		if (node == nullptr)
		{
			return black == 0;
		}
		if (!IsRed(node))
		{
			black--;
		}
		return IsBalanced(node->left.get(), black) && IsBalanced(node->right.get(), black);
	}

	const bool IsSizeConsistent(NodeType* node) const noexcept
	{
		if (node == nullptr)
		{
			return true;
		}

		if (node->sub_node_num != Size(node->left.get()) + Size(node->right.get()) + 1)
		{
			return false;
		}

		return IsSizeConsistent(node->left.get()) && IsSizeConsistent(node->right.get());
	}

	UniqueNodeType DelMin(UniqueNodeType node)
	{
		if (node->left == nullptr)
		{
			return nullptr;
		}
		if (!IsRed(node->left.get()) && !IsRed(node->left->left.get()))
		{
			node = MoveRedLeft(std::move(node));
		}

		node->left = DelMin(std::move(node->left));

		return Balance(std::move(node));
	}

	UniqueNodeType DelMax(UniqueNodeType node)
	{
		if (IsRed(node->left.get()))
		{
			node = RotateRight(std::move(node));
		}

		if (node->right == nullptr)
		{
			return nullptr;
		}

		if (!IsRed(node->right.get()) && !IsRed(node->right->left.get()))
		{
			node = MoveRedRight(std::move(node));
		}

		node->right = DelMax(std::move(node->right));

		return Balance(std::move(node));
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

	NodeType const*const Select(NodeType* node, int ranking)const noexcept
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
	int Rank(NodeType* node, NodeValType&& val)const noexcept
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
		if (nullptr == node)
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

		std::stack<NodeType*> tmp_stack, out_stack;

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

	template<typename NodeValType>
	UniqueNodeType Delete(UniqueNodeType node, NodeValType&& val)
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (val < node->val)
		{
			if (node->left != nullptr && !IsRed(node->left.get()) && !IsRed(node->left->left.get()))
			{
				node = MoveRedLeft(std::move(node));
			}
			node->left = Delete(std::move(node->left),std::forward<NodeValType>(val));
		}
		else
		{
			if (IsRed(node->left.get()))
			{
				node = RotateRight(std::move(node));
			}
			if (val == node->val && node->right == nullptr)
			{
				return nullptr;
			}
			if (node->right != nullptr && !IsRed(node->right.get()) && !IsRed(node->right->left.get()))
			{
				node = MoveRedRight(std::move(node));
			}
			if (val == node->val)
			{
				NodeType* min_node = Min(node->right.get());
				if (min_node != nullptr)
				{
					node->val = min_node->val;
					node->right = DelMin(std::move(node->right));
				}
			}
			else
			{
				node->right = Delete(std::move(node->right),std::forward<NodeValType>(val));
			}
		}

		return Balance(std::move(node));
	}

	const bool IsRed(NodeType* node)const noexcept
	{
		if (node == nullptr)
		{
			return false;
		}

		return node->color == NodeType::RED;
	}

	const int Size(NodeType* node)const noexcept
	{
		if (node == nullptr)
		{
			return 0;
		}

		return node->sub_node_num;
	}

	UniqueNodeType RotateRight(UniqueNodeType node)noexcept
	{
		if (node == nullptr)
		{
			return node;
		}
		if (!IsRed(node->left.get()))
		{
			return node;
		}

		UniqueNodeType tmp = std::move(node->left);
		node->left = std::move(tmp->right);
		tmp->sub_node_num = node->sub_node_num;
		node->sub_node_num = Size(node->left.get()) + Size(node->right.get()) + 1;
		tmp->right = std::move(node);
		tmp->color = tmp->right->color;
		tmp->right->color = NodeType::RED;

		return tmp;
	}

	UniqueNodeType RotateLeft(UniqueNodeType node)
	{
		if (node == nullptr)
		{
			return node;
		}
		if (!IsRed(node->right.get()))
		{
			return node;
		}

		UniqueNodeType tmp = std::move(node->right);
		node->right = std::move(tmp->left);
		tmp->sub_node_num = node->sub_node_num;
		node->sub_node_num = Size(node->left.get()) + Size(node->right.get()) + 1;
		tmp->left = std::move(node);
		tmp->color = tmp->left->color;
		tmp->left->color = NodeType::RED;

		return tmp;
	}

	void FlipColor(NodeType* node)noexcept
	{
		if (!(node != nullptr &&
			node->left != nullptr &&
			node->right != nullptr))
		{
			return;
		}

		if ((!IsRed(node) && IsRed(node->left.get()) && IsRed(node->right.get())) || 
			(IsRed(node) && !IsRed(node->left.get()) && !IsRed(node->right.get())))
		{
			node->color = !node->color;
			node->left->color = !node->left->color;
			node->right->color = !node->right->color;
		}
	}

	UniqueNodeType Balance(UniqueNodeType node)
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (IsRed(node->right.get()))
		{
			node = RotateLeft(std::move(node));
		}

		if (IsRed(node->left.get()) && IsRed(node->left->left.get()))
		{
			node = RotateRight(std::move(node));
		}

		if (IsRed(node->left.get())&& IsRed(node->right.get()))
		{
			FlipColor(node.get());
		}

		node->sub_node_num = Size(node->left.get()) + Size(node->right.get()) + 1;

		return node;
	}

	UniqueNodeType MoveRedLeft(UniqueNodeType node)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		if (node->left == nullptr)
		{
			return node;
		}

		if (!(IsRed(node.get())&&!IsRed(node->left.get())&&!IsRed(node->left->left.get())))
		{
			return node;
		}

		FlipColor(node.get());
		if (node->right != nullptr && IsRed(node->right->left.get()))
		{
			node->right = RotateRight(std::move(node->right));
			node = RotateLeft(std::move(node));
			FlipColor(node.get());
		}

		return node;
	}

	UniqueNodeType MoveRedRight(UniqueNodeType node)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		if (node->right == nullptr)
		{
			return node;
		}
		if (!(IsRed(node.get())&& !IsRed(node->right.get()) && !IsRed(node->right->left.get())))
		{
			return node;
		}

		FlipColor(node.get());
		if (node->left != nullptr && IsRed(node->left->left.get()))
		{
			node = RotateRight(std::move(node));
			FlipColor(node.get());
		}

		return node;
	}
	

private:
	UniqueNodeType root_;
};

template<>
class RedBlackBST<float> {};

template<>
class RedBlackBST<double> {};

#endif // !TREE_H_
