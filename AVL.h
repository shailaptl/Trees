/*
*
* Created by shaila:
* Date: Dec 1, 2017
*
*/

#include "Map.h"
#include <iostream>

namespace cop3530{
template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
//all info given during initialization of object.
class AVL:public Map <K, V, comp_func, equals_func>
{
	//the CF comparison func and EF equality func should relate to the K type.
	public:
		Node<K, V>* root;

		AVL();
		void insert(K key, V val);
		void remove(K key);
		V& lookup(K key);
		bool contains(K key);
		bool is_empty();
		bool is_full();
		int size();
		int size(Node<K,V>* temp);
		void clear();
		int height();
		int balance();

		void insert(K key, V val, Node<K,V>*& here);
		void remove(K key, Node<K, V>*& here);
		V& lookup(K key, Node<K, V>* here);
		void RRrotate(Node<K,V>* here);
		void LLrotate(Node<K,V>* here);
		void LRrotate(Node<K,V>* here);
		void RLrotate(Node<K,V>* here);
		int height(Node<K, V>* here);
		void rotate(Node<K, V>* here);

		//BIG 5
		AVL(const AVL& old) //copy constructor
		{
			root = new Node<K, V>(old.root);
		}
		~AVL()	{	delete root;	}//will recursively get rid of all of the Nodes.
		AVL operator=(AVL& old)
		{
			root = new Node<K, V>(old.root); 		//creates a new Node will recursively make new Nodes until nullptr.
			return this;
		}
		AVL(AVL&& other)		//move constructor, doesnt return anything.
		{
			root = other.root;
			other.root = nullptr;
		}
		AVL& operator=(AVL&& other)		//move assignment operator
		{
			root = other.root;
			other.root = nullptr;
			return *this;
		}
};

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
AVL<K, V, comp_func, equals_func>::AVL()
{
	root = nullptr;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void AVL<K, V, comp_func, equals_func>::rotate(Node<K, V>* here)		//checks if needs to rotate before rotates.
{
	if(height(here) == 2)
	{
		if(height(here->left) == 1)
			LLrotate(here);
		else if(height(here->left) == -1)
			LRrotate(here);
	}
	else if(height(here) == -2)
	{
		if(height(here->right) == -1)
			RRrotate(here);
		else if(height(here->right) == 1)
			RLrotate(here);
	}
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void AVL<K, V, comp_func, equals_func>::insert(K key, V val)
{
	if(!root)
	{
		root = new Node<K, V>(key, val);
		return;
	}
	insert(key, val, root);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void AVL<K, V, comp_func, equals_func>::insert(K key, V val, Node<K,V>*& here)
{
	//depending on val, insert key there.
	if(!here)
	{
		here = new Node<K, V>(key, val);
		return;
	}
	else if(comp_func(key, here->key))	//key comes before root, therefore key should be placed on left
	{
		if(here->left)
		{
			insert(key, val, here->left);
		//	rotate(here);
		}
		else
		{
			here->left = new Node<K, V>(key, val);
			return;
		}
	}
	else if(equals_func(key, here->key))
	{
		here->value = val;
	}
	else
	{
		if(here->right)
		{
			insert(key, val, here->right);
		//	rotate(here);
		}
		else
		{
			here->right = new Node<K, V>(key, val);
			return;
		}
	}
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void AVL<K, V, comp_func, equals_func>::remove(K key)
{
	this->remove(key, root);
}


template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void AVL<K, V, comp_func, equals_func>::remove(K key, Node<K, V>*& here)
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
	rotate(here);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
V& AVL<K, V, comp_func, equals_func>::lookup(K key)
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
V& AVL<K, V, comp_func, equals_func>::lookup(K key, Node<K, V>* here)
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
void AVL<K, V, comp_func, equals_func>::RRrotate(Node<K,V>* here)
{
	Node<K, V>* temp = here->right;
	here->right = temp->left;
	temp->left = here;
	here = temp;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void AVL<K, V, comp_func, equals_func>::LLrotate(Node<K,V>* here)
{
	Node<K, V>* temp = here->left;
	here->left = temp->right;
	temp->right = here;
	here = temp;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void AVL<K, V, comp_func, equals_func>::LRrotate(Node<K,V>* here)
{
	RRrotate(here->left);
	LLrotate(here);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
void AVL<K, V, comp_func, equals_func>::RLrotate(Node<K,V>* here)
{
	LLrotate(here->right);
	RRrotate(here);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int AVL<K, V, comp_func, equals_func>::height(Node<K, V>* here)
{
	if(!here)
		return 0;
	int max = height(here->left) > height(here->right) ? height(here->left) : height(here->right);
	return 1 + max;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
bool AVL<K, V, comp_func, equals_func>::contains(K key)
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
bool AVL<K, V, comp_func, equals_func>::is_empty()
{
	if(!root)
		return false;
	return true;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
bool AVL<K, V, comp_func, equals_func>::is_full()
{
	return false;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int AVL<K, V, comp_func, equals_func>::size()
{
	return size(root);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int AVL<K, V, comp_func, equals_func>::size(Node<K,V>* here)
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
void AVL<K, V, comp_func, equals_func>::clear()
{
	delete root;
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int AVL<K, V, comp_func, equals_func>::height()
{
	return height(root);
}

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
int AVL<K, V, comp_func, equals_func>::balance()
{
	return height(root->left) - height(root->right);
}
}
