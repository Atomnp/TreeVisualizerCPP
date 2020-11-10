#pragma once

#include <iostream>
#include "info.h"
#include <mutex>
struct RBTNode {

	//for graphics
	int height;
	//for pure data structure
	int data;
	RBTNode* left;
	RBTNode* right;
	bool isRed;

	RBTNode(int data) :data(data) {
		isRed = true;
		left = nullptr;
		right = nullptr;

	}
};
class RedBlackTree
{
private:
	std::mutex m;

public:
	
	RBTNode* root;
public:

	RedBlackTree() {
		root = nullptr;
	}
	void insert(int n) {
		
		info::currentInsertingItem = n;
		root = insert(root, n);
		root->isRed = false;
	}

	void flipColors(RBTNode* node) {
		printf("flip color");
		node->left->isRed =!node->left->isRed;
		node->right->isRed = !node->right->isRed;
		node->isRed = node == root ? false:true;
	}
	bool isRed(RBTNode* node) {
		if (node == nullptr)return false;
		return node->isRed;
	}
	void printInorder() {
		printInorder(root);
	}
	void remove(int n) {
		auto temp = remove(root, n);
		root = temp;
	}

	void print2D()
	{
		// Pass initial space count as 0  
		//print2DUtil(root, 0);
	}
	RBTNode* insert(RBTNode* x, int n) {
		info::rbtnode = x;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (x == nullptr) {
			x = new RBTNode(n);
			if (root == nullptr)x->isRed = false;
			return x;
		}
		else if (n <= x->data) {

			x->left = insert(x->left, n);
		}
		else if (n > x->data) {
			x->right = insert(x->right, n);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (isRed(x->right) && !isRed(x->left)) {
			x = singleLeftRotate(x);
		}
		
		if (isRed(x->left) && isRed(x->left->left)) {
			x = singleRightRotate(x);
		}
		if (isRed(x->left) && isRed(x->right)) {
			flipColors(x);
		}
		return x;
	}
	RBTNode* singleRightRotate(RBTNode* x) {

		RBTNode* temp = x->left;
		bool  tempc = x->left->isRed;
		x->left = temp->right;
		
		temp->isRed = x->isRed;
		x->isRed = tempc;

		temp->right = x;
		
		return temp;

	}
	RBTNode* singleLeftRotate(RBTNode* x) {


		RBTNode* temp = x->right;
		bool  tempc = x->right->isRed;
		x->right = temp->left;
		temp->isRed = x->isRed;
		x->isRed = tempc;

		temp->left = x;
		return temp;

	}
	int max(int i, int j) {
		if (i <= j)return j;
		return i;
	}
	RBTNode* findMin(RBTNode* t) {
		if (t == nullptr)
			return nullptr;
		else if (t->left == nullptr)
			return t;
		else
			return findMin(t->left);
	}

	RBTNode* findMax(RBTNode* t) {
		if (t == nullptr)
			return nullptr;
		else if (t->right == nullptr)
			return t;
		else
			return findMax(t->right);
	}
	void printInorder(RBTNode* x) {
		if (x == nullptr)return;
		printInorder(x->left);
		//std::cout << x->data << " ";
		printInorder(x->right);
	}

	RBTNode* remove(RBTNode* x, int n) {
		info::rbtnode = x;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		//when element is not found in the tree
		if (x == nullptr)return nullptr;

		//when delete node key is less then key we are current standing at search left
		if (n < x->data) {
			x->left = remove(x->left, n);
			info::deleting = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		//when delete node key is greater then key we are current standing at search right
		else if (n > x->data) {
			x->right = remove(x->right, n);
			info::deleting = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		//when we find the deleting node
		else {
			info::deleting = true;
			//if both child are present
			if (x->left && x->right) {
				RBTNode* temp = x;
				x->right->left = x->left->right;
				x->left->right = x->right;
				x = x->left;
				delete temp;
			}
			//when deleting node is leaf node
			else if (x->left == nullptr and x->right == nullptr) {
				delete x;
				return nullptr;
			}
			//when only right child is present
			else if (x->left == nullptr) {
				RBTNode* temp = x;
				x = x->right;
				delete temp;
			}
			//when only left child is present
			else if (x->right == nullptr) {
				RBTNode* temp = x;
				x = x->left;
				delete temp;
			}
			

		}

		if (isRed(x->right) && !isRed(x->left))x = singleLeftRotate(x);
		if (isRed(x->left) && isRed(x->left->left))x = singleRightRotate(x);
		if (isRed(x->left) && isRed(x->right))flipColors(x);

		return x;

	}
};

