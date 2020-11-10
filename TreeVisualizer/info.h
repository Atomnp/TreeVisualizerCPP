#pragma once
#include <atomic>
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;


static const int CONTROL_MENU_WIDTH = SCREEN_WIDTH;
static const int CONTROL_MENU_HEIGHT = 150;

static const int SORTING_MENU_WIDTH = 200;
static const int SORTING_MENU_HEIGHT = SCREEN_HEIGHT - CONTROL_MENU_HEIGHT;

//struct Node {
//	int data;
//	Node* left;
//	Node* right;
//	int height;
//	Node(int x, int h) :data(x), height(h) {
//		left = nullptr;
//		right = nullptr;
//	}
//};
class Node;
class RBTNode;
namespace info {
	int currentInsertingItem;
	bool done = false;
	bool treeThreadActive=false;
	Node* currentNode;
	RBTNode* rbtnode;
	int timeMilli;
	int windowFlags = 0;
	std::string displayMessage = "Binary Search Tree";
	std::atomic<bool>deleting=false;
	
	//Point moveTo;
	
}