#pragma once
#include <atomic>
#include "Node.h"
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

static const int CONTROL_MENU_WIDTH = SCREEN_WIDTH;
static const int CONTROL_MENU_HEIGHT = 150;

static const int SORTING_MENU_WIDTH = 200;
static const int SORTING_MENU_HEIGHT = SCREEN_HEIGHT - CONTROL_MENU_HEIGHT;

//base class for bst, avl and RBT
class BinarySearchTree {
public:
	virtual Node* getRoot() = 0;
	virtual void insert(int) = 0;
	virtual void remove(int) = 0;

};
namespace info {
	int currentInsertingItem;
	bool done = false;
	bool treeThreadActive=false;
	Node* currentNode;
	int timeMilli;
	int windowFlags = 0;
	std::string displayMessage = "Binary Search Tree";
	std::atomic<bool>deleting(false);
	int currentTree = 1;
}