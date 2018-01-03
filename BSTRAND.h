/*
*
* Created byshail: 
* Date: Dec 1, 2017
*
*/
#include "Map.h"
#include <iostream>
#include <cstdlib>
namespace cop3530{

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>	//all info given during initialization of object.
class BSTRAND: public Map<K, V, comp_func, equals_func>
{
	//K = type of number for Key
	//V = value type
	//CF = comparison function
	//EF = equality function
	public:
		//the CF comparison func and EF equality func should relate to the K type.
		Node<K,V>* root;

		BSTRAND();
		void insert(K key, V val);
		void insert(K mkey, V val, Node<K, V>*& here, const bool flip);
		void remove(K key);
		void remove(K key, Node<K, V>*& here);
		V& lookup(K key);
		V& lookup(K key, Node<K, V>* here);
		int numberOfLeafs(Node<K, V>* temp);
		Node<K,V>* RRrotate(Node<K,V>* here);
		Node<K,V>* LLrotate(Node<K,V>* here);
		
		bool contains(K key);
		bool is_empty();
		bool is_full();
		int size();
		int size(Node<K,V>* temp);
		void clear();
		int height();
		int height(Node<K, V>* here);
		int balance();

		//BIG 5
		BSTRAND(const BSTRAND& old) //copy constructor
		{
			root = new Node<K, V>(old.root);
		}
		~BSTRAND()	{	delete root;	}//will recursively get rid of all of the Nodes.
		BSTRAND operator=(BSTRAND& old)
		{
			this->root = new Node<K, V>(old.root); 		//creates a new Node will recursively make new Nodes until nullptr.
			return this;
		}
		BSTRAND(BSTRAND&& other)		//move constructor, doesnt return anything.
		{
			root = other.root;
			other.root = nullptr;
		}
		BSTRAND& operator=(BSTRAND&& other)		//move assignment operator
		{
			root = other.root;
			other.root = nullptr;
			return *this;
		}
};

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
BSTRAND<K, V, comp_func, equals_func>::BSTRAND()
{
	root = nullptr;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void BSTRAND<K, V, comp_func, equals_func>::insert(K key, V val)
{
	//Find out how many leaves
	if(!root)
		{
			root = new Node<K, V>(key, val);
			return;
		}
	int leaves = numberOfLeafs(root);
	int randNum = rand() % (leaves+1);
	std::cout<< rand() << " " << leaves << "  " << randNum << std::endl;
	bool f = (randNum == 0);			//if true, then you have to insert at the root, and you flip
	insert(key, val, this->root, f);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void BSTRAND<K, V, comp_func, equals_func>::insert(K mkey, V val, Node<K, V>*& here, const bool flip)
{	//depending on val, insert key there.
	if(!here)
	{
		here = new Node<K, V>(mkey, val);
		return;
	}
	else if(comp_func(mkey, here->key))	//key comes before root, therefore key should be placed on left
	{
		if(here->left)
		{
			insert(mkey, val, here->left, flip);
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
			insert(mkey, val, here->right, flip);
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
int BSTRAND<K, V, comp_func, equals_func>::numberOfLeafs(Node<K,V>* temp)
{
	if(temp->left && temp->right)
		return numberOfLeafs(temp->left) + numberOfLeafs(temp->right);
	else if(temp->left || temp->right)
	{
		Node <K,V>* hold = temp->left ? temp->left : temp->right;
		return numberOfLeafs(hold);
	}
	else
		return 1;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void BSTRAND<K, V, comp_func, equals_func>::remove(K key)
{
	remove(key, root);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void BSTRAND<K, V, comp_func, equals_func>::remove(K key, Node<K, V>*& here)
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
V& BSTRAND<K, V, comp_func, equals_func>::lookup(K key)
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
V& BSTRAND<K, V, comp_func, equals_func>::lookup(K key, Node<K, V>* here)
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
int BSTRAND<K, V, comp_func, equals_func>::height(Node<K, V>* here)
{
	if(!here)
		return 0;
	int max = height(here->left) > height(here->right) ? height(here->left) : height(here->right);
	return 1 + max;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
bool BSTRAND<K, V, comp_func, equals_func>::contains(K key)
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
bool BSTRAND<K, V, comp_func, equals_func>::is_empty()
{
	if(!root)
		return false;
	return true;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
bool BSTRAND<K, V, comp_func, equals_func>::is_full()
{
	return false;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int BSTRAND<K, V, comp_func, equals_func>::size()
{
	return size(root);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int BSTRAND<K, V, comp_func, equals_func>::size(Node<K,V>* here)
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
void BSTRAND<K, V, comp_func, equals_func>::clear()
{
	delete root;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int BSTRAND<K, V, comp_func, equals_func>::height()
{
	return height(root);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int BSTRAND<K, V, comp_func, equals_func>::balance()
{
	return height(root->left) - height(root->right);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
Node<K,V>* BSTRAND<K, V, comp_func, equals_func>::RRrotate(Node<K,V>* here)
{
	Node<K, V>* temp = here;
	here = here->right;
	temp->right = here->left;
	here->left = temp;
	return here;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
Node<K,V>* BSTRAND<K, V, comp_func, equals_func>::LLrotate(Node<K,V>* here)
{
	Node<K, V>* temp = here;
	here = here->left;
	temp->left = here->right;
	here->right = temp;
	return here;
}
}
