#pragma once

template<typename T>
class RedBlackTree final
{
public:
	RedBlackTree();
	~RedBlackTree();

	void	Insert(T value);
	bool	Remove(T value);
	bool	IsRedBlackTree();
	void	Print();

private:
	enum class eColor
	{
		Black,
		Red
	};

	class Node
	{
	public:
		Node() = default;
		Node(T value);
		Node* GetGrandParent();
		Node* GetUncle();
		Node* GetSibling();

		T Value;
		eColor Color;
		Node* Parent;
		Node* Left;
		Node* Right;
	};

	void rotateLeft(Node* node);
	void rotateRight(Node* node);
	void insertFixupRecursive(Node* node);
	void removeFixupRecursive(Node* node);
	void destroyTreeRecursive(Node* root);
	bool isRedBlackTreeRecursive(Node* node, int* blackDepth);
	void printNode(Node* node, bool bIsRight, std::string indent);
private:
	static Node NilNode;
	Node* mRoot;
	size_t mSize;
};

template<typename T>
typename RedBlackTree<T>::Node RedBlackTree<T>::NilNode;

template<typename T>
inline RedBlackTree<T>::RedBlackTree()
	: mRoot(&NilNode),
	  mSize(0)
{
}

template<typename T>
inline RedBlackTree<T>::~RedBlackTree()
{
	if (mRoot != nullptr)
	{
		destroyTreeRecursive(mRoot);
	}
}

template<typename T>
inline void RedBlackTree<T>::Insert(T value)
{
	Node* newNode = new Node(value);
	Node* parent = mRoot;
	Node* insertPos = mRoot;

	while (insertPos != &NilNode)
	{
		parent = insertPos;
		if (newNode->Value <= parent->Value)
		{
			insertPos = parent->Left;
		}
		else
		{
			insertPos = parent->Right;
		}
	}
	
	if (insertPos == mRoot)
	{
		mRoot = newNode;
	}
	else if (newNode->Value <= parent->Value)
	{
		parent->Left = newNode;
	}
	else
	{
		parent->Right = newNode;
	}
	newNode->Parent = parent;
	++mSize;

	insertFixupRecursive(newNode);
}

template<typename T>
inline bool RedBlackTree<T>::Remove(T value)
{
	Node* target = mRoot;
	Node* removed;
	Node* child;
	Node* replaced;

	while (target != &NilNode)
	{
		if (target->Value == value)
		{
			break;
		}
		else if (value < target->Value)
		{
			target = target->Left;
		}
		else
		{
			target = target->Right;
		}
	}
	if (target == &NilNode)
	{
		return false;
	}
	removed = target;
	child = target->Right;

	while (child != &NilNode)
	{
		removed = child;
		child = child->Left;
	}
	target->Value = removed->Value;

	if (removed->Left == &NilNode)
	{
		replaced = removed->Right;
	}
	else
	{
		replaced = removed->Left;
	}
	replaced->Parent = removed->Parent;

	if (removed->Parent->Left == removed)
	{
		removed->Parent->Left = replaced;
	}
	else
	{
		removed->Parent->Right = replaced;
	}

	if (removed == mRoot)
	{
		mRoot = replaced;
	}

	if (removed->Color == eColor::Black)
	{
		if (replaced->Color == eColor::Red)
		{
			replaced->Color = eColor::Black;
		}
		else
		{
			removeFixupRecursive(replaced);
		}
	}
	delete removed;
	--mSize;

	return true;
}

template<typename T>
inline bool RedBlackTree<T>::IsRedBlackTree()
{
	int depth;

	if (mRoot->Color != eColor::Black)
	{
		return false;
	}

	return isRedBlackTreeRecursive(mRoot, &depth);
}

template<typename T>
inline void RedBlackTree<T>::Print()
{
	std::string indent = "";

	if (mRoot->Right != &NilNode)
	{
		printNode(mRoot->Right, true, indent);
	}
	std::string color = mRoot->Color == eColor::Black ? "Black" : "Red";
	std::cout << '[' << color << ']' << "value : " << mRoot->Value << std::endl;
	if (mRoot->Left != &NilNode)
	{
		printNode(mRoot->Left, false, indent);
	}
}

template<typename T>
inline void RedBlackTree<T>::insertFixupRecursive(Node* node)
{
	if (node == mRoot)
	{
		node->Color = eColor::Black;
		return;
	}
	else if (node->Parent->Color == eColor::Black)
	{
		return;
	}

	Node* grandParent = node->GetGrandParent();
	Node* uncle = node->GetUncle();

	if (uncle->Color == eColor::Red)
	{
		uncle->Color = eColor::Black;
		node->Parent->Color = eColor::Black;
		grandParent->Color = eColor::Red;

		insertFixupRecursive(grandParent);
		return;
	}

	if (node == node->Parent->Right && node->Parent == grandParent->Left)
	{
		rotateLeft(node->Parent);
		node = node->Left;
	}
	else if (node == node->Parent->Left && node->Parent == grandParent->Right)
	{
		rotateRight(node->Parent);
		node = node->Right;
	}

	node->Parent->Color = eColor::Black;
	grandParent->Color = eColor::Red;

	if (node == node->Parent->Left)
	{
		rotateRight(grandParent);
	}
	else
	{
		rotateLeft(grandParent);
	}
}

template<typename T>
inline void RedBlackTree<T>::removeFixupRecursive(Node* node)
{
	Node* sibling;

	if (node == mRoot)
	{
		return;
	}
	
	sibling = node->GetSibling();

	if (sibling->Color == eColor::Red)
	{
		node->Parent->Color = eColor::Red;
		sibling->Color = eColor::Black;

		if (node == node->Parent->Left)
		{
			rotateLeft(node->Parent);
		}
		else
		{
			rotateRight(node->Parent);
		}
	}
	
	sibling = node->GetSibling();

	if (node->Parent->Color == eColor::Black &&
		sibling->Color == eColor::Black &&
		sibling->Left->Color == eColor::Black &&
		sibling->Right->Color == eColor::Black)
	{
		sibling->Color = eColor::Red;
		removeFixupRecursive(node->Parent);
		return;
	}

	if (node->Parent->Color == eColor::Red &&
		sibling->Color == eColor::Black &&
		sibling->Left->Color == eColor::Black &&
		sibling->Right->Color == eColor::Black)
	{
		sibling->Color = eColor::Red;
		node->Parent->Color = eColor::Black;
		return;
	}

	if (sibling->Color == eColor::Black)
	{
		if (node == node->Parent->Left &&
			sibling->Right->Color == eColor::Black &&
			sibling->Left->Color == eColor::Red)
		{
			sibling->Color = eColor::Red;
			sibling->Left->Color = eColor::Black;
			rotateRight(sibling);
		}
		else if (node == node->Parent->Right &&
			sibling->Left->Color == eColor::Black &&
			sibling->Right->Color == eColor::Red)
		{
			sibling->Color = eColor::Red;
			sibling->Right->Color = eColor::Black;
			rotateLeft(sibling);
		}
	}

	sibling = node->GetSibling();

	sibling->Color = node->Parent->Color;
	node->Parent->Color = eColor::Black;

	if (node == node->Parent->Left)
	{
		sibling->Right->Color = eColor::Black;
		rotateLeft(node->Parent);
	}
	else
	{
		sibling->Left->Color = eColor::Black;
		rotateRight(node->Parent);
	}
}

template<typename T>
inline void RedBlackTree<T>::rotateLeft(Node* node)
{
	Node* parent = node->Parent;
	Node* child = node->Right;

	if (node == mRoot)
	{
		mRoot = child;
	}
	if (child->Left != &NilNode)
	{
		child->Left->Parent = node;
	}
	node->Right = child->Left;
	node->Parent = child;
	child->Left = node;
	child->Parent = parent;

	if (parent->Left == node)
	{
		parent->Left = child;
	}
	else
	{
		parent->Right = child;
	}
}

template<typename T>
inline void RedBlackTree<T>::rotateRight(Node* node)
{
	Node* parent = node->Parent;
	Node* child = node->Left;

	if (node == mRoot)
	{
		mRoot = child;
	}
	if (child->Right != &NilNode)
	{
		child->Right->Parent = node;
	}
	node->Left = child->Right;
	node->Parent = child;
	child->Right = node;
	child->Parent = parent;

	if (parent->Left == node)
	{
		parent->Left = child;
	}
	else
	{
		parent->Right = child;
	}
}

template<typename T>
inline void RedBlackTree<T>::destroyTreeRecursive(Node* root)
{
	if (root == &NilNode)
	{
		return;
	}
	destroyTreeRecursive(root->Left);
	destroyTreeRecursive(root->Right);
	delete root;
}

template<typename T>
inline bool RedBlackTree<T>::isRedBlackTreeRecursive(Node* node, int* blackDepth)
{
	bool bIsLeftValid;
	bool bIsRightValid;
	int leftBlackDepth = 0;
	int rightBlackDepth = 0;

	if (node == &NilNode)
	{
		return true;
	}

	if (node->Color == eColor::Black)
	{
		*blackDepth += 1;
	}
	else if (node->Left->Color == eColor::Red
			|| node->Right->Color == eColor::Red)
	{
		return false;
	}
	bIsLeftValid = isRedBlackTreeRecursive(node->Left, &leftBlackDepth);
	bIsRightValid = isRedBlackTreeRecursive(node->Right, &rightBlackDepth);

	*blackDepth += leftBlackDepth;
	if (leftBlackDepth != rightBlackDepth)
	{
		assert(leftBlackDepth == rightBlackDepth);
	}
	return bIsLeftValid
		&& bIsRightValid
		&& leftBlackDepth == rightBlackDepth;
}

template<typename T>
inline void RedBlackTree<T>::printNode(Node* node, bool bIsRight, std::string indent)
{
	if (node->Right != &NilNode)
	{
		printNode(node->Right, true, indent + (bIsRight ? "        " : " |      "));
	}
	std::cout << indent;
	if (bIsRight)
	{
		std::cout << " /";
	}
	else
	{
		std::cout << " L";
	}
	std::cout << "----- ";
	std::string color = node->Color == eColor::Black ? "Black" : "Red";
	std::cout << '[' << color << ']' << "value : " << node->Value << std::endl;
	if (node->Left != &NilNode)
	{
		printNode(node->Left, false, indent + (bIsRight ? " |      " : "        "));
	}
}

template<typename T>
inline RedBlackTree<T>::Node::Node(T value)
	: Value(value),
	  Color(eColor::Red),
	  Parent(&NilNode),
	  Left(&NilNode),
	  Right(&NilNode)
{
}

template<typename T>
inline typename RedBlackTree<T>::Node* RedBlackTree<T>::Node::GetGrandParent()
{
	if (Parent != &NilNode)
	{
		return Parent->Parent;
	}
	return &NilNode;
}

template<typename T>
inline typename RedBlackTree<T>::Node* RedBlackTree<T>::Node::GetUncle()
{
	Node* grandParent = GetGrandParent();

	if (grandParent == &NilNode)
	{
		return &NilNode;
	}
	if (grandParent->Left == Parent)
	{
		return grandParent->Right;
	}
	return grandParent->Left;;
}

template<typename T>
inline typename RedBlackTree<T>::Node* RedBlackTree<T>::Node::GetSibling()
{
	if (Parent->Left == this)
	{
		return Parent->Right;
	}
	return Parent->Left;
}
