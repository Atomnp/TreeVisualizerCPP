#pragma once
#include <iostream>
#include <thread>
#include <chrono>

#include <mutex>
#include "info.h"
#include "Node.h"

class BST :public BinarySearchTree {

private:	
	int count;
private:
	Node* deleteMinimum(Node* node,int &toReturn) {
		if (node->left == nullptr) {
			toReturn = node->data;
			return node->right;	
		}	 
		node->left=deleteMinimum(node->left, toReturn);
		return node;	

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
			Node* newNode = new Node(x);
			return newNode;
		}
		else if (x < r->data) {
			r->left = insert(r->left, x);
			
		}
		else {
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
	Node* getRoot() override { return root; }
	int getCount() { return count; }
	Node* root;
	BST() :root(nullptr), count(0) {}
	void insert(int x) override {
		root=insert(root, x);
		count++;
	}
	void remove(int x) override {
		root=remove(root, x);
		count--;
	}
	void clear() {
		while (count != 0) {
			remove(root->data);
		}
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