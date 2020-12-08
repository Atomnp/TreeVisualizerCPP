#pragma once
#include "../info.h"
#include <chrono>
#include<thread>
#include <iostream>
#include "../Node.h"

class AVL: public BinarySearchTree
{
public:
	Node* root;
public:
	Node* getRoot() override {
		return root;
	}
	AVL() {
		root = nullptr;
	}
	void insert(int n) override {
		info::currentInsertingItem = n;
		root = insert(root, n);
	}
	int height(Node* x) {
		return x == nullptr ? -1 : x->height;
	}
	void remove(int n) override {
		info::currentRemovingItem = n;
		root = remove(root, n);
		info::deleting = false;
	}

	Node* singleRightRotate(Node* x) {

		Node* temp = x->left;
		x->left = temp->right;
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		temp->right = x;
		x->height = max(height(x->left), height(x->right)) + 1;
		temp->height = max(height(temp->left), x->height) + 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		return temp;
	}

	Node* singleLeftRotate(Node* x) {

		Node* temp = x->right;
		x->right = temp->left;
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		temp->left = x;
		x->height = max(height(x->left), height(x->right)) + 1;
		temp->height = max(height(temp->right), x->height) + 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		return temp;

	}
	int max(int i, int j) {
		if (i <= j)return j;
		return i;
	}
	Node* doubleLeftRotate(Node* x) {
		x->right = singleRightRotate(x->right);
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		return  singleLeftRotate(x);
	}
	Node* doubleRightRotate(Node* x) {
		x->left = singleLeftRotate(x->left);
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		return singleRightRotate(x);
	}

	Node* findMin(Node* t) {
		if (t == nullptr)
			return nullptr;
		else if (t->left == nullptr)
			return t;
		else
			return findMin(t->left);
	}

	Node* findMax(Node* t) {
		if (t == nullptr)
			return nullptr;
		else if (t->right == nullptr)
			return t;
		else
			return findMax(t->right);
	}
	Node* insert(Node* x, int n) {
		info::currentNode = x;
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		if (x == nullptr) {
			x = new Node(n);
		}
		//if item being inserted is less than current node insert it its left sub tree
		else if (n <= x->data) {
			x->left = insert(x->left, n);
		}
		//if item being inserted is greater then insert in the right subtree
		else if (n > x->data) {
			x->right = insert(x->right, n);
		}
		x->height = max(height(x->left), height(x->right)) + 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		if (height(x->left) - height(x->right) >= 2) {
			//item was inserted to left of left
			if (n <= x->left->data) {
				x = singleRightRotate(x);
			}
			else {
				x = doubleRightRotate(x);
			}
		}
		else if (height(x->right) - height(x->left) >= 2) {
			//item was to the right of right
			if (n > x->right->data) {
				x = singleLeftRotate(x);
			}
			//item was inserted to the left of right
			else {
				x = doubleLeftRotate(x);
			}
		}
		//std::cout << "heignt of " <<" root "<<"="<<height(root)<< std::endl;
		return x;
	}
	Node* remove(Node* x, int n) {
		info::currentNode = x;
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		//when element is not found in the tree
		if (x == nullptr)return nullptr;

		//when delete node key is less then key we are current standing at search left
		if (n < x->data) {
			x->left = remove(x->left, n);
			info::deleting = false;
		}
		//when delete node key is greater then key we are current standing at search right
		else if (n > x->data) {
			x->right = remove(x->right, n);
			info::deleting = false;
		}
		//when we find the deleting node
		else {
			//if both child are present
			if (x->left && x->right) {
				Node* temp = x;
				auto minNode = findMin(x->right);
				x->data = minNode->data;
				x->right = remove(x->right, minNode->data);
				info::deleting = false;
			}
			//when deleting node is leaf node
			else if (x->left == nullptr and x->right == nullptr) {
				info::deleting = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				Node* temp = x;
				x = nullptr;
				delete temp;

				return nullptr;
			}
			//when only right child is present
			else if (x->left == nullptr) {
				info::deleting = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				Node* temp = x;
				x = x->right;
				delete temp;
			}
			//when only left child is present
			else if (x->right == nullptr) {
				info::deleting = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				Node* temp = x;
				x = x->left;
				delete temp;
			}

		}
		x->height = max(height(x->left), height(x->right)) + 1;
		//std::cout << "x->data=" << x->data << std::endl;
		//now we have to balance the tree;
		if (height(x->left) - height(x->right) >= 2) {
			//when left has more height than right
			//we have to rotate 3 node one is x, left child of x and next is left of left of x 
			//or right of left of x

			//now i may have to do ll rotate or lr rotate
			if (height(x->left->left) >= height(x->left->right)) {
				//perform ll rotate
				x = singleRightRotate(x);
			}
			if (height(x->left->left) < height(x->left->right)) {
				//perform lR rotate
				x = doubleRightRotate(x);
			}
		}
		else if (height(x->right) - height(x->left) >= 2) {
			if (height(x->right->right) > height(x->right->left)) {
				//perform rr rotate
				x = singleLeftRotate(x);
			}
			else if (height(x->right->right) < height(x->right->left)) {
				//perform RL rotate
				x = doubleLeftRotate(x);
			}
		}
		return x;
	}
};
