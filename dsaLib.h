/*
* =========================================================================================
* Name        : dsaLib.h
* Author      : Duc Dung Nguyen
* Email       : nddung@hcmut.edu.vn
* Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
* Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
* =========================================================================================
*/
#pragma region //Definition
#ifndef A02_DSALIB_H
#define A02_DSALIB_H

#include <string>
#include <math.h>
#include <vector>
//#pragma warning(disable:4715)
using namespace std;
#pragma endregion
#pragma region //DSAException
class DSAException
{
	int _error;
	string _text;

  public:
	DSAException() : _error(0), _text("Success") {}
	DSAException(int err) : _error(err), _text("Unknown Error") {}
	DSAException(int err, const char *text) : _error(err), _text(text) {}

	int getError() { return _error; }
	string &getErrorText() { return _text; }
};
#pragma endregion
template <class T>
struct L1Item
{
	T data;
	L1Item<T> *pNext;
	L1Item() : pNext(NULL) {}
	L1Item(T &a) : data(a), pNext(NULL) {}
};

template <class T>
class L1List
{
	L1Item<T> *pHead = NULL;
	size_t size = 0;

  public:
	L1List(){};
	~L1List()
	{
		while (pHead)
			removeHead();
	};

	bool isEmpty()
	{
		return pHead == NULL;
	}

	L1Item<T> *Head()
	{
		return pHead;
	}

	size_t Size()
	{
		return size;
	}
	void insertHead(T &value)
	{
		L1Item<T> *tmp = new L1Item<T>(value);
		tmp->pNext = pHead;
		pHead = tmp;
		size++;
	}

	void removeHead()
	{
		if (pHead != NULL)
		{
			L1Item<T> *tmp = pHead;
			pHead = pHead->pNext;
			delete tmp;
			size--;
		}
	}

	void reverse()
	{
		L1List<T> *tmp = new L1List<T>();
		while (pHead)
		{
			tmp->insertHead(pHead->data);
			removeHead();
		}
		pHead = tmp->Head();
		size = tmp->Size();
	}

	void traverse(void (*op)(T &))
	{
		for (L1Item<T> *p = pHead; p != NULL; p = p->pNext)
		{
			op(p->data);
		}
	}

	T &operator[](int index)
	{
		int i = index;
		L1Item<T> *p = pHead;
		for (; i > 0 && p != NULL;)
		{
			i--;
			p = p->pNext;
		}
		if (i > 0 || p == NULL)
			throw("Access violation");
		return p->data;
	}
};
/************************************************************************
* This section is for AVL tree
************************************************************************/
#define NROOT 0
#define NLEFT 1
#define NRIGHT 2
template <class T>
struct AVLNode
{
	T data;
	AVLNode<T> *LChild = NULL;
	AVLNode<T> *RChild = NULL;
	AVLNode<T> *Father = NULL;

	int Height = 1;
	int Weight = 1;
	char Type;

	AVLNode(T &value) : data(value) {}
	AVLNode(AVLNode<T> &obj) : data(obj->value) {}

	//Return true if the node is leaf node
	bool IsLeaf()
	{
		return LChild == RChild == NULL;
	}

	//Return diffrence of left and right child node height
	int Equilibrium()
	{
		int LHeight = 0;
		int RHeight = 0;
		if (LChild)
			LHeight = LChild->Height;
		if (RChild)
			RHeight = RChild->Height;
		return LHeight - RHeight;
	}
};

template <class T>
class AVLTree
{
	AVLNode<T> *pRoot;

  public:
	AVLTree() : pRoot(NULL) {}
	~AVLTree() { Destroy(pRoot); }

	//Check if the tree is empty
	bool IsEmpty()
	{
		return pRoot == NULL;
	}

	//Return a pointer to the value at root node
	T *Root()
	{
		return &(pRoot->data);
	}

	//Return the tree's weight
	size_t Weight()
	{
		return pRoot->Weight;
	}
	//Look for a value in the tree at root, return a pointer to the found enement. If not found, return NULL
	virtual T *Find(T &value) { return Find(pRoot, value); }

	//Insert a new element to the tree, return 1 if success
	void Insert(T &key) { Insert(pRoot, key); }

	//Remove an exist element in the tree, return 1 if success, or 0 if not found
	bool Remove(T &key) { return Remove(pRoot, key); }

	//Traverse the tree NLR
	void TraverseNLR(void (*op)(T &)) { TraverseNLR(op, pRoot); }
	//Traverse the tree LNR
	void TraverseLNR(void (*op)(T &)) { TraverseLNR(op, pRoot); }
	//Traverse the tree LRN
	void TraverseLRN(void (*op)(T &)) { TraverseLRN(op, pRoot); }

	//Traverse the tree NLR
	void TraverseNLR(void (*op)(T &, void *), void *param) { TraverseNLR(op, param, pRoot); }
	//Traverse the tree LNR
	void TraverseLNR(void (*op)(T &, void *), void *param) { TraverseLNR(op, param, pRoot); }
	//Traverse the tree LRN
	void TraverseLRN(void (*op)(T &, void *), void *param) { TraverseLRN(op, param, pRoot); }

  protected:
	//Return the rightmost node of the tree at root
	AVLNode<T> *RightMost(AVLNode<T> *root)
	{
		if (root == NULL)
			return NULL;
		if (root->RChild)
			return RightMost(root->RChild);
		else
			return root;
	}
	//Destroy the node and all of its sub-node
	void Destroy(AVLNode<T> *node)
	{ // Destroy the node but without updating the tree
		if (node == NULL)
			return;
		if (node->LChild)
			Destroy(node->LChild);
		if (node->RChild)
			Destroy(node->RChild);

		switch (node->Type)
		{
		case NROOT:
			break;
		case NLEFT:
			node->Father->LChild = NULL;
			break;
		case NRIGHT:
			node->Father->RChild = NULL;
			break;
		default:
			throw("DSA Error");
			break;
		}
		/*
		if (node->Father) {
			Update(node->Father);
			BalanceUp(node->Father);
		}
		*/
		delete node;
	};

	//Look for a value in the tree at root, return a pointer to the found element. If not found, return NULL
	T *Find(AVLNode<T> *root, T &key)
	{
		if (root == NULL)
			return NULL;
		else if (root->data == key)
			return &root->data;
		else if (root->data < key)
			return Find(root->RChild, key);
		else
			return Find(root->LChild, key);
	}

	//Look for the node with have the value, return NULL if not found
	AVLNode<T> *FindNode(AVLNode<T> *root, T &key)
	{
		if (root == NULL)
			return NULL;
		else if (root->data == key)
			return root;
		else if (root->data < key)
			return FindNode(root->RChild, key);
		else
			return FindNode(root->LChild, key);
	}

	//Update the height and weight of the node
	void Update(AVLNode<T> *node)
	{
		if (node == NULL)
			return;
		//Update height
		node->Height = 1;
		if (node->LChild)
			node->Height = (node->Height > 1 + node->LChild->Height ? node->Height : 1 + node->LChild->Height);
		if (node->RChild)
			node->Height = (node->Height > 1 + node->RChild->Height ? node->Height : 1 + node->RChild->Height);

		//Update weight
		node->Weight = 1;
		if (node->LChild)
			node->Weight += node->LChild->Weight;
		if (node->RChild)
			node->Weight += node->RChild->Weight;

		//Recursive
		if (node->Father)
			Update(node->Father);
	}

	//Rotate the node to the left, the node must have its right child not null
	void RotateLeft(AVLNode<T> *node)
	{
		AVLNode<T> *a = node;
		AVLNode<T> *c = node->RChild;

		//Re-link
		c->Father = a->Father;
		a->RChild = c->LChild;
		if (a->RChild)
			a->RChild->Father = a;
		c->LChild = a;
		a->Father = c;

		//Update type
		c->Type = a->Type;
		a->Type = NLEFT;
		if (a->RChild)
			a->RChild->Type = NRIGHT;

		//Relink from top father node
		switch (c->Type)
		{
		case NLEFT:
			c->Father->LChild = c;
			break;
		case NRIGHT:
			c->Father->RChild = c;
			break;
		case NROOT:
			pRoot = c;
			break;
		default:
			break;
		}

		//Update height and weight
		Update(a);
	};
	//Rotate the node to the right, the node must have its left child not null
	void RotateRight(AVLNode<T> *node)
	{
		AVLNode<T> *a = node;
		AVLNode<T> *b = a->LChild;

		//Re-link
		b->Father = a->Father;
		a->LChild = b->RChild;
		if (a->LChild)
			a->LChild->Father = a;
		b->RChild = a;
		a->Father = b;

		//Update type
		b->Type = a->Type;
		a->Type = NRIGHT;
		if (a->LChild)
			a->LChild->Type = NLEFT;
		if (b->Type == NROOT)
			pRoot = b;

		//Relink from the top father node
		switch (b->Type)
		{
		case NLEFT:
			b->Father->LChild = b;
			break;
		case NRIGHT:
			b->Father->RChild = b;
			break;
		case NROOT:
			pRoot = b;
			break;
		default:
			break;
		}

		//Update height and weight
		Update(a);
	};

	//Rebalance the node, return true if the node is rotated
	bool Balance(AVLNode<T> *node)
	{
		if (node == NULL)
			return false;
		if (node->Height >= 3)
		{
			//LL
			if (node->Equilibrium() > 1 && node->LChild->Equilibrium() >= 0)
			{
				RotateRight(node);
				return true;
			}

			//LR
			else if (node->Equilibrium() > 1 && node->LChild->Equilibrium() < 0)
			{
				RotateLeft(node->LChild);
				RotateRight(node);
				return true;
			}

			//RR
			else if (node->Equilibrium() < -1 && node->RChild->Equilibrium() <= 0)
			{
				RotateLeft(node);
				return true;
			}

			//RL
			else if (node->Equilibrium() < -1 && node->RChild->Equilibrium() > 0)
			{
				RotateRight(node->RChild);
				RotateLeft(node);
				return true;
			}
		}
		return false;
	}

	//Rebalance up
	void BalanceUp(AVLNode<T> *node)
	{
		if (node == NULL)
			return;
		if (!Balance(node))
			BalanceUp(node->Father);
	}

	//Insert a new node to the tree at pR root
	void Insert(AVLNode<T> *pR, T &a)
	{
		if (pR == NULL)
		{ //The root is empty, create new pRoot
			AVLNode<T> *tmp = new AVLNode<T>(a);
			//Node properties
			tmp->Type = NROOT;
			tmp->Height = 1;
			tmp->Weight = 1;
			pRoot = tmp;
			pR = tmp;
		}
		else if (pR->data >= a && pR->LChild == NULL)
		{
			AVLNode<T> *tmp = new AVLNode<T>(a);
			//Node properties
			tmp->Type = NLEFT;
			tmp->Father = pR;
			pR->LChild = tmp;
			Update(tmp);
		}
		else if (pR->data < a && pR->RChild == NULL)
		{
			AVLNode<T> *tmp = new AVLNode<T>(a);
			//Node properties
			tmp->Type = NRIGHT;
			tmp->Father = pR;
			pR->RChild = tmp;
			Update(tmp);
		}
		else if (pR->data >= a)
			Insert(pR->LChild, a);
		else if (pR->data < a)
			Insert(pR->RChild, a);
		//After inserting, rebalance the node
		BalanceUp(pR);
	}

	//Remove a node from the tree at pR root, return 1 if completed, or 0 if fail or not found
	bool Remove(AVLNode<T> *pR, T &a)
	{
		AVLNode<T> *tmp = FindNode(pR, a);
		if (tmp)
		{ //Found node to delete
			AVLNode<T> *rm = RightMost(tmp->LChild);

			if (rm != NULL)
			{ //If found rightmost node of left subtree, swap its value with current node
				T tmpT = rm->data;
				bool ret = Remove(rm, rm->data);
				tmp->data = tmpT;
				return ret;
			}
			else // Left subtree is NULL
			{
				switch (tmp->Type)
				{
				case NROOT:
					if (tmp->RChild)
					{
						pRoot = tmp->RChild;
						tmp->RChild->Type = NROOT;
						tmp->RChild->Father = NULL;
					}
					else
						pRoot = NULL;
					break;
				case NLEFT:
					tmp->Father->LChild = tmp->RChild;
					if (tmp->RChild)
					{
						tmp->RChild->Father = tmp->Father;
						tmp->RChild->Type = NLEFT;
					}
					break;
				case NRIGHT:
					tmp->Father->RChild = tmp->RChild;
					if (tmp->RChild)
						tmp->RChild->Father = tmp->Father;
					break;
				default:
					throw("DSA Error");
					break;
				}
				Update(tmp->Father);
				BalanceUp(tmp->Father);
				delete tmp;
			}
			return true;
		}
		else
			return false;
	};

	//Traverse the tree in LNR order with function void op
	void TraverseLNR(void (*op)(T &), AVLNode<T> *node)
	{
		if (node == NULL)
			return;
		if (node->LChild)
			TraverseLNR(op, node->LChild);
		op(node->data);
		if (node->RChild)
			TraverseLNR(op, node->RChild);
	};
	//Traverse the tree in NLR order with function void op
	void TraverseNLR(void (*op)(T &), AVLNode<T> *node)
	{
		if (node == NULL)
			return;
		op(node->data);
		if (node->LChild)
			TraverseNLR(op, node->LChild);
		if (node->RChild)
			TraverseNLR(op, node->RChild);
	};
	//Traverse the tree in LRN order with function void op
	void TraverseLRN(void (*op)(T &), AVLNode<T> *node)
	{
		if (node == NULL)
			return;
		if (node->LChild)
			TraverseLRN(op, node->LChild);
		if (node->RChild)
			TraverseLRN(op, node->RChild);
		op(node->data);
	};

	//Traverse the tree in LNR order with function void op
	void TraverseLNR(void (*op)(T &, void *), void *param, AVLNode<T> *node)
	{
		if (node == NULL)
			return;
		if (node->LChild)
			TraverseLNR(op, param, node->LChild);
		op(node->data, param);
		if (node->RChild)
			TraverseLNR(op, node->RChild);
	};
	//Traverse the tree in NLR order with function void op
	void TraverseNLR(void (*op)(T &, void *), void *param, AVLNode<T> *node)
	{
		if (node == NULL)
			return;
		op(node->data, param);
		if (node->LChild)
			TraverseNLR(op, param, node->LChild);
		if (node->RChild)
			TraverseNLR(op, param, node->RChild);
	};
	//Traverse the tree in LRN order with function void op
	void TraverseLRN(void (*op)(T &, void *), void *param, AVLNode<T> *node)
	{
		if (node == NULL)
			return;
		if (node->LChild)
			TraverseLRN(op, param, node->LChild);
		if (node->RChild)
			TraverseLRN(op, param, node->RChild);
		op(node->data, param);
	};
};
/************************************************************************
* This section is for Heap
************************************************************************/
template <class T>
class Heap
{
	vector<T> data;

	bool (*comp)(T &, T &);

	int FatherNode(int x)
	{
		return ((x + 1) / 2 - 1);
	}
	int LeftNode(int x)
	{
		return 2 * x + 1;
	}
	int RightNode(int x)
	{
		return 2 * x + 2;
	}

  public:
};
#endif //A02_DSALIB_H
