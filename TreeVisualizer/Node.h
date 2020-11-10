#pragma once
#include <iostream>

struct Node {

	//for graphics
	int height;
	//for pure data structure
	int data;
	Node* left;
	Node* right;
	bool isRed;

	Node(int data):data(data){
		isRed = true;
		left = nullptr;
		right = nullptr;

	}
};
