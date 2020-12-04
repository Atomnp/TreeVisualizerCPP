#include "app.h"
#include "utils.h"
#include "graph.h"
#undef main

void createTree(BinarySearchTree*& tree,int numberOfItems) {
	std::vector<int>arr = generateRandomArray(numberOfItems);
	printArray(arr);
	info::timeMilli = 100;
	for (auto elm : arr) {
		tree->insert(elm);
	}
}

void insertItem(BinarySearchTree*& tree, int value) {
	tree->insert(value);
	info::currentNode = nullptr;
}
void removeItem(BinarySearchTree*& tree, int value) {
	tree->remove(value);
	info::currentNode = nullptr;
}
void App::run() {
	BinarySearchTree* tree = new AVL();
	Graph graph;
	
	while (!info::done) {

		handleEvents();
		ClearScreen();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		bool b = false;

		//ImGui::ShowDemoWindow();

		ImGui::SetNextWindowPos(ImVec2(0, SCREEN_HEIGHT - CONTROL_MENU_HEIGHT));
		ImGui::SetNextWindowSize(ImVec2(CONTROL_MENU_WIDTH, CONTROL_MENU_HEIGHT));
		info::windowFlags |= ImGuiWindowFlags_NoResize;


		
		ImGui::Begin("Controller", &b, info::windowFlags);
		ImGui::InputInt("Number of items", &info::numberOfItems);
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			switch (info::currentTree)
			{
			//case for bst
			case 0:
				if (t1.joinable()) {
					info::timeMilli = 0;
					t1.join();
				}
				delete tree;
				tree = new BST();
				break;
			//case for avl tree
			case 1:
				if (t1.joinable()) {
					info::timeMilli = 0;
					t1.join();
				}
				delete tree;
				tree = new AVL();
				break;
			//case for red black tree
			case 2:
				if (t1.joinable()) {
					info::timeMilli = 0;
					t1.join();
				}
				delete tree;
				tree = new RedBlackTree();
				break;
			default:
				break;
			}
			info::treeThreadActive = false;

		}
		if (!info::treeThreadActive) {
			t1 = std::thread(createTree, std::ref(tree),info::numberOfItems);
			info::treeThreadActive = true;
		}
		if (info::treeThreadActive) {
			graph.draw(tree, renderer);

		}
		static int valueToInsert = 123;
		ImGui::InputInt("Item To Insert", &valueToInsert);
		ImGui::SameLine();
		if (ImGui::Button("insert")) {
			//when this key is pressed insert should happen;
			if (t1.joinable()) {
				info::timeMilli = 0;
				t1.join();
			}
			info::timeMilli = 100;
			t1 = std::thread(insertItem, std::ref(tree), valueToInsert);
		}

		static int valueToRemove = 0;
		ImGui::InputInt("Item To Remove", &valueToRemove);
		ImGui::SameLine();
		if (ImGui::Button("Remove")) {
			//when this key is pressed insert should happen;
			if (t1.joinable()) {
				info::timeMilli = 0;
				t1.join();
			}
			info::timeMilli = 10;
			t1 = std::thread(removeItem, std::ref(tree), valueToRemove);
		}
		ImGui::End();
		ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - SORTING_MENU_WIDTH, 0));
		ImGui::SetNextWindowSize(ImVec2(SORTING_MENU_WIDTH, SORTING_MENU_HEIGHT));

		ImGui::Begin("TREE", NULL, info::windowFlags);
		const char* items[] = {
			"Binary Search Tree",
			"AVL tree",
			"Red Black Tree",
		};
		ImGui::ListBox("", &info::currentTree, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items));
		ImGui::End();
		render();
	}
}

int main() {
	auto app = new App();
	app->run();
	return EXIT_SUCCESS;
}