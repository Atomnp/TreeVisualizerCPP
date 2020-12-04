#include "app.h"
#include "utils.h"
#include "graph.h"
#undef main

void createTree(BinarySearchTree *&tree, int numberOfItems)
{
	std::vector<int> arr = generateRandomArray(numberOfItems);
	printArray(arr);
	for (auto elm : arr)
	{
		tree->insert(elm);
	}
}
void insertItem(BinarySearchTree *&tree, int value)
{
	tree->insert(value);
	info::currentNode = nullptr;
}
void removeItem(BinarySearchTree *&tree, int value)
{
	tree->remove(value);
	info::currentNode = nullptr;
}
void App::run()
{
	BinarySearchTree *tree = new AVL();
	Graph graph;

	while (!info::done)
	{

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
		std::cout<<"tree thread active is"<<info::treeThreadActive<<std::endl;
		ImGui::SliderInt("Number of Items", &info::numberOfItems, 1, 30);	
		ImGui::SameLine();

		if (ImGui::Button("Reset"))
		{
			if (t1.joinable()){
				int temp = info::timeMilli;
				info::timeMilli = 0;
				t1.join();
				info::timeMilli = temp;
			}
			info::treeThreadActive = false;
			delete tree;
			switch (info::currentTree)
			{
			//case for bst
			case 0:
				tree = new BST();
				break;
			//case for avl tree
			case 1:
				tree = new AVL();
				break;
			//case for red black tree
			case 2:
				tree = new RedBlackTree();
				break;
			default:
				break;
			}
		}
		if (!info::treeThreadActive)
		{	
			info::treeThreadActive = true;
			t1 = std::thread(createTree, std::ref(tree), info::numberOfItems);
			
		}
		if (info::treeThreadActive)
		{
			graph.draw(tree, renderer);
		}
		static int valueToInsert = 123;
		ImGui::InputInt("Item To Insert", &valueToInsert);
		ImGui::SameLine();
		if (ImGui::Button("insert"))
		{
			//when this key is pressed insert should happen;
			if (t1.joinable())
			{
				int temp = info::timeMilli;
				info::timeMilli = 0;
				t1.join();
				info::timeMilli = temp;
			}
			t1 = std::thread(insertItem, std::ref(tree), valueToInsert);
		}

		static int valueToRemove = 0;
		ImGui::InputInt("Item To Remove", &valueToRemove);
		ImGui::SameLine();
		if (ImGui::Button("Remove"))
		{
			//when this key is pressed insert should happen;
			if (t1.joinable())
			{
				int temp = info::timeMilli;
				info::timeMilli = 0;
				t1.join();
				info::timeMilli = temp;
			}
			t1 = std::thread(removeItem, std::ref(tree), valueToRemove);
		}
		if (info::treeThreadActive)
		{
			ImGui::SliderInt("Delay", &info::timeMilli, 0, 1000);
		}
		ImGui::End();
		ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - SORTING_MENU_WIDTH, 0));
		ImGui::SetNextWindowSize(ImVec2(SORTING_MENU_WIDTH, SORTING_MENU_HEIGHT));

		ImGui::Begin("TREE", NULL, info::windowFlags);
		const char *items[] = {
			"Binary Search Tree",
			"AVL tree",
			"Red Black Tree",
		};
		ImGui::ListBox("", &info::currentTree, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items));
		ImGui::End();
		render();
	}
}

int main()
{
	auto app = new App();
	app->run();
	return EXIT_SUCCESS;
}