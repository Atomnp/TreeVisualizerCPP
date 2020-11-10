
#include "app.h"
#include "AVL.h"
#include "utils.h"
#include "AVLGraph.h"
#include "app.h"
#undef main

void createTree(AVL& bst) {
    std::vector<int>arr = { 47, 94, 8, 75, 47, 23, 23, 14, 48, 90 };
    info::timeMilli = 10;
    for (auto elm : arr) {
        bst.insert(elm);
    }
}

void insertItem(AVL& bst, int value) {
    bst.insert(value);
    info::currentNode = nullptr;
}
void removeItem(AVL& bst, int value) {
    bst.remove(value);
    info::currentNode = nullptr;
}
void App::run() {

    AVL bst;
    AVLGraph bg;
    if (!info::treeThreadActive) {
        t1 = std::thread(createTree, std::ref(bst));
        info::treeThreadActive = true;
    }
    SDL_Rect rec = { 100,100,100,100 };

    while (!info::done) {

        handleEvents();
        ClearScreen();
        bg.draw(bst, renderer);
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
        static int valueToInsert = 123;
        ImGui::InputInt("Item To Insert", &valueToInsert);
        ImGui::SameLine();
        if (ImGui::Button("insert")) {
            //when this key is pressed insert should happen;
            if (t1.joinable()) {
                info::timeMilli = 0;
                t1.join();
            }
            info::timeMilli = 10;
            t1 = std::thread(insertItem, std::ref(bst), valueToInsert);
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
            t1 = std::thread(removeItem, std::ref(bst), valueToRemove);
        }
        ImGui::End();

        render();
    }
}


int main() {
    auto app = new App();
    app->run();
    return EXIT_SUCCESS;
}