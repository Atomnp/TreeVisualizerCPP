#pragma once
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;


static const int CONTROL_MENU_WIDTH = SCREEN_WIDTH;
static const int CONTROL_MENU_HEIGHT = 150;

static const int SORTING_MENU_WIDTH = 200;
static const int SORTING_MENU_HEIGHT = SCREEN_HEIGHT - CONTROL_MENU_HEIGHT;

struct Node {
	int data;
	Node* left;
	Node* right;
	int height;
	Node(int x, int h) :data(x), height(h) {
		left = nullptr;
		right = nullptr;
	}
};
namespace info {
	bool done = false;
	bool treeThreadActive=false;
	Node* currentNode;
	int timeMilli =100;
	ImGuiWindowFlags windowFlags = 0;
	std::string displayMessage = "Binary Search Tree";
	//Point moveTo;
	
}