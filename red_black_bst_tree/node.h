#ifndef NODE_H_
#define NODE_H_

#include <memory>

template<typename T>
struct Node
{
	Node() = delete;
	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;

	const static bool RED = true;
	const static bool BLACK = false;

	template<typename NodeValType>
	explicit Node(NodeValType&& param) :left(nullptr), right(nullptr), val(std::forward<NodeValType>(param)), sub_node_num(1),color(RED)
	{
	}

	std::unique_ptr<Node<T>> left;
	std::unique_ptr<Node<T>> right;

	T val;

	// 以该节点为根的子树中的节点总数
	int sub_node_num;
	// 节点颜色
	bool color;
};

#endif // !NODE_H_
