/*
*
* Created byshail: 
* Date: Dec 1, 2017
*
*/

#include "Map.h"
#include <iostream>
namespace cop3530{
template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
class BSTROOT:public Map<K, V, comp_func, equals_func>
{
	public:
		Node<K, V>* root;

		BSTROOT();
		void insert(K key, V val);
		void insert(K key, V val, Node<K,V>*& here);
		void remove(K key);
		void remove(K key, Node<K, V>*& here);
		V& lookup(K key);
		V& lookup(K key, Node<K, V>* here);
		Node<K,V>* RRrotate(Node<K,V>* here);
		Node<K,V>* LLrotate(Node<K,V>* here);

		bool contains(K key);
		bool is_empty();
		bool is_full();
		int size();
		int size(Node<K,V>* temp);
		void clear();
		int height();
		int height(Node<K,V>* here);
		int balance();
	//BIG 5
	BSTROOT(const BSTROOT& old) //copy constructor
	{
		root = new Node<K, V>(old.root);
	}
	~BSTROOT()	{	delete root;	}//will recursively get rid of all of the Nodes.
	BSTROOT operator=(BSTROOT& old)
	{
		root = new Node<K, V>(old.root); 		//creates a new Node will recursively make new Nodes until nullptr.
		return this;
	}
	BSTROOT(BSTROOT&& other)		//move constructor, doesnt return anything.
	{
		root = other.root;
		other.root = nullptr;
	}
	BSTROOT& operator=(BSTROOT&& other)		//move assignment operator
	{
		root = other.root;
		other.root = nullptr;
		return *this;
	}
};

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
BSTROOT<K, V, comp_func, equals_func>::BSTROOT()
{
	root = nullptr;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void BSTROOT<K, V, comp_func, equals_func>::insert(K mkey, V val)
{
	if(!root)
	{
		root = new Node<K, V>(mkey, val);
		return;
	}
	insert(mkey, val, root);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void BSTROOT<K, V, comp_func, equals_func>::insert(K mkey, V val, Node<K, V>*& here)
{
	//depending on val, insert key there.
	if(!here)
	{
		here = new Node<K, V>(mkey, val);
		return;
	}
	else if(comp_func(mkey, here->key))	//key comes before root, therefore key should be placed on left
	{
		if(here->left)
		{
			insert(mkey, val, here->left);
			here = LLrotate(here);
		}
		else
		{
			here->left = new Node<K, V>(mkey, val);
			here = LLrotate(here);
			return;
		}
	}
	else if(equals_func(mkey, here->key))
	{
		here->value = val;
	}
	else
	{
		if(here->right)
		{
			insert(mkey, val, here->right);
			here = RRrotate(here);
		}
		else
		{
			here->right = new Node<K, V>(mkey, val);
			here = RRrotate(here);
			return;
		}
	}
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void BSTROOT<K, V, comp_func, equals_func>::remove(K key)
{
	this->remove(key, this->root);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void BSTROOT<K, V, comp_func, equals_func>::remove(K key, Node<K, V>*& here)
//Deletes a node from within given "here" Node.
//returns root of modified tree (rotates before returns)
{
	if(!here)	//if root is null, return it. Nothing removed.
	{
		std::cout<< key << " has not been found." << std::endl;
		return;
	}
	else if(comp_func(key, here->key))
		remove(key, here->left);
	else if(comp_func(here->key, key))
		remove(key, here->right);
	else
	{
		if(!here->right && !here->left)					//NO CHILDREN
		{
			Node<K,V>* temp = here;
			here = nullptr;
			delete temp;
			return;
		}
		else if(!here->right || !here->left)			//ONLY ONE CHILD
		{
			Node<K, V>* temp = here->left ? here->left : here->right;
			*here = *temp;		//so you dont have to go find the parent.
			return;
		}
		else											//2 CHILDREN
		{
			//find the Node with the smallest key on the right side.
			Node<K, V>* temp = here->right;
			while(temp->left)
			{
				temp = temp->left;
			}
			here->key = temp->key;
			here->value = temp->value;
			remove(temp->key, here->right);		//it should not have any children.
		}
	}
	if(!root)
		return;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
V& BSTROOT<K, V, comp_func, equals_func>::lookup(K key)
{
	try
	{
		if(this->root)
			return lookup(key, root);
		else
			throw(99);
	}
	catch(int i)
	{
		std::cout<< key << " does not exist!";
	}
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
V& BSTROOT<K, V, comp_func, equals_func>::lookup(K key, Node<K, V>* here)
{
	try
	{
		if(!here)
			throw 99;
		if(equals_func(key, here->key))
			return here->value;
		else if(comp_func(key, here->key))
			return lookup(key, here->left);
		else
			return lookup(key, here->right);
	}
	catch(int i)
	{
		std::cout<<"There is no such key." << std::endl;
	}
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
Node<K,V>* BSTROOT<K, V, comp_func, equals_func>::RRrotate(Node<K,V>* here)
{
	Node<K, V>* temp = here;
	here = here->right;
	temp->right = here->left;
	here->left = temp;
	return here;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
Node<K,V>* BSTROOT<K, V, comp_func, equals_func>::LLrotate(Node<K,V>* here)
{
	Node<K, V>* temp = here;
	here = here->left;
	temp->left = here->right;
	here->right = temp;
	return here;
}

template <typename K, typename V>
void print(Node<K,V>* temp)
{
//	if(temp)
//	{
//		std::cout<< temp->key << " ";
//		print(temp->right);
//		print(temp->left);
//	}
	std::cout << temp->key << " ";
	if(temp->left)
	{
		std::cout << "L ";
		print(temp->left);
	}
	if(temp->right)
	{
		std::cout<< "R ";
		print(temp->right);
	}
}


template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int BSTROOT<K, V, comp_func, equals_func>::height(Node<K, V>* here)
{
	if(!here)
		return 0;
	int max = height(here->left) > height(here->right) ? height(here->left) : height(here->right);
	return 1 + max;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
bool BSTROOT<K, V, comp_func, equals_func>::contains(K key)
{
	Node<K,V>* temp = root;
	while(temp)
	{
		if(equals_func(key, temp->key))
			return true;
		else if(comp_func(key, temp->key))
			temp = temp->left;
		else
			temp = temp->right;
	}
	return false;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
bool BSTROOT<K, V, comp_func, equals_func>::is_empty()
{
	if(!root)
		return false;
	return true;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
bool BSTROOT<K, V, comp_func, equals_func>::is_full()
{
	return false;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int BSTROOT<K, V, comp_func, equals_func>::size()
{
	return size(root);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int BSTROOT<K, V, comp_func, equals_func>::size(Node<K,V>* here)
{
	if(!here->left && !here->right)
		return 1;
	else if(here->left && !here->right)
		return 1+size(here->left);
	else if(!here->left && here->right)
		return 1+size(here->right);
	else
		return 1+size(here->right) + size(here->left);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void BSTROOT<K, V, comp_func, equals_func>::clear()
{
	delete root;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int BSTROOT<K, V, comp_func, equals_func>::height()
{
	return height(root);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int BSTROOT<K, V, comp_func, equals_func>::balance()
{
	return height(root->left) - height(root->right);
}
}
