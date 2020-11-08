#pragma once
#include <iostream>
#include <thread>
#include <chrono>

#include <mutex>
#include "info.h"



class BST {

private:	
	int count;
	int tempHeight;

private:
	Node* deleteMinimum(Node* node,int &toReturn) {
		if (node->left == nullptr) {
			toReturn = node->data;
			decreeMentHeight(node->right);
			return node->right;	
		}	 
		node->left=deleteMinimum(node->left, toReturn);
		return node;	

	}
	void decreeMentHeight(Node* node) {
		if (node == nullptr)return;
		node->height--;
		decreeMentHeight(node->left);
		decreeMentHeight(node->right);
	}
	int max(int a, int b) {
		return a >= b ? a : b;
	}
	Node* maxof(Node* a, Node* b) {
		if (a == nullptr)return b;
		if (b == nullptr)return a;
		if (a->data > b->data)return a;
		return b;
	}

	Node* insert(Node* r, int x) {
		info::currentNode = r;
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		if (r == nullptr) {
			Node* newNode = new Node(x,tempHeight);
			tempHeight = 1;
			return newNode;
		}
		else if (x < r->data) {
			tempHeight++;
			r->left = insert(r->left, x);
			
		}
		else {
			tempHeight++;
			r->right = insert(r->right, x);
			
		}
		return r;
	}
	Node* remove(Node* r, int x) {
		info::currentNode = r;
		std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
		if (r == nullptr) {
			std::cout << "no such data exist" << std::endl;
			info::displayMessage = " NOT FOUND!!";
			return r;
		}
		if (x < r->data) {
			r->left = remove(r->left, x);
			return r;
		}
		else if (x > r->data) {
			r->right = remove(r->right, x);
			return r;
		}
		else {
			//when the node being deleted has only one child
			if (r->left == nullptr or r->right == nullptr) {
				Node* toReturn = maxof(r->left, r->right);
				decreeMentHeight(toReturn);
				delete r;
				return toReturn;
			}
			else {
				//delete min and get Data of the minimum and set r->data to 
				//that minimum value
				r->right=deleteMinimum(r->right,r->data);
				return r;
			}
		}
	}
public:
	int getCount() { return count; }
	Node* root;
	BST() :root(nullptr), count(0),tempHeight(1) {}
	void insert(int x) {
		tempHeight = 1;
		if (root == nullptr) {
			root = new Node(x,tempHeight);
		}
		else {
			info::currentNode = root;
			std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
			if (x < root->data) {
				tempHeight++;
				root->left = insert(root->left, x);
				
			}
			else {
				tempHeight++;
				root->right = insert(root->right, x);

				
			}
		}
		count++;
	}
	void remove(int x) {
		if (root == nullptr) {
			std::cout << "cant remove form empty tree" << std::endl;
		}
		else if(x!=root->data) {
			info::currentNode = root;
			if (x < root->data) {
				std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
				root->left = remove(root->left, x);
			}
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(info::timeMilli));
				root->right = remove(root->right, x);
			}
		}
		else {
			if (root->left == nullptr or root->right == nullptr) {
				Node* max = maxof(root->left, root->right);
				decreeMentHeight(max);
				delete root;
				root = max;
			}
			else root->right = deleteMinimum(root->right, root->data);
			
		}
		count--;
	}
	void clear() {
		while (count != 0) {
			remove(root->data);
		}
	}
	int heightOf(Node* node) {
		if (node == nullptr)return -1;
		return node->height;
	}
	int height(Node* node) {
		if (node == nullptr)return 0;
		return (1 + max(height(node->left), height(node->right)));
	}
	int treeHeight() {
		if (root == nullptr)return 0;
		return (1 + max(height(root->left), height(root->right)));
	}
};
//unit test

//int main() {
//	BST b;
//	b.insert(20);
//	b.insert(30);
//	b.insert(15);
//	b.insert(5);
//	b.insert(19);
//	b.insert(21);
//	b.insert(55);
//	b.remove(15);
//}