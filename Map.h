/*
 * Map.h
 *
 *  Created on: Dec 6, 2017
 *      Author: shail
 */

#ifndef MAP_H_
#define MAP_H_

template <typename K, typename V, bool (*comp_func)(const K& y, const K& z), bool (*equals_func)(const K& y, const K& z)>
class Map
{
	public:
		void insert(K key, V val);
		void remove(K key);
		V& lookup(K key);
};

template <typename K, typename V>
class Node
{
	public:
		K key;
		V value;
		Node* right;
		Node* left;
		//constructor
		Node(K k,V val);
		//BIG 5
		Node(const Node& old) //copy constructor, takes in the old as a parameter
		{
			if(old->right)
				this->right = new Node(old->right);		//should call copy constructor for Node.
			else
				this->right = nullptr;

			if(old->left)
				this->left = new Node(old->left);
			else
				this->left = nullptr;

			this->key = old->key;
			this->value = old->value;
		}
		~Node()	{	delete left;	delete right;	}
};

template <typename K, typename V>
Node<K,V>::Node(K k, V val)
{
	key = k;
	value = val;
	right = nullptr;
	left = nullptr;
}

#endif /* MAP_H_ */
