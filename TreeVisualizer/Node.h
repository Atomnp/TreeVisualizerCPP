#pragma once
#include <iostream>

class Node {
public:
	//for graphics
	int height;
	//for pure data structure
	int data;
	Node* left;
	Node* right;
	bool isRed;

	Node(int data):data(data),height(0){
		isRed = true;
		left = nullptr;
		right = nullptr;

	}
};
