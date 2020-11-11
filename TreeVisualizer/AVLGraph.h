#pragma once
#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <queue>
#include <cmath>
#include <string>


#include "info.h"
#include "utils.h"
#include "AVL.h"

struct Point {
    int x;
    int y;
    Point(int x, int y) :x(x), y(y) {}
};

class AVLGraph {
private:
    const int rectWidth = 2 * 2 * 8;
    const int rectHeight = 2 * 2 * 8;
    const int VerticalGap = 32;
    const int lastGap = 2 * 2 * 16 - rectWidth;

    int treeHeight;
    TTF_Font* font;
    SDL_Surface* surfaceMessage;
    SDL_Texture* message;
    SDL_Color color;
public:

    AVLGraph() {
        TTF_Init();
        font = TTF_OpenFont("Sans.ttf", 24); //this opens a font style and sets a size
        color = { 255, 255, 255 };
    }

    //height from top or root
    //calculates what should be the gap between nodes at given height
    int gapAtHeight(int h) {
        return lastGap * pow(2, treeHeight - h + 1);
    }
    //calculate initial offset of the each height
    int initialOffsetAT(int height) {

        if (height == treeHeight)return 0;
        if (treeHeight - height == 1) {
            return (rectWidth + (gapAtHeight(height + 1))) / 2 - rectWidth / 2;
        }
        return initialOffsetAT(height + 1) + (rectWidth + (gapAtHeight(height + 1))) / 2 - rectWidth / 2;
    }
    //returns the position of the node based on its count
    Point getPos(int count) {
        //1,2,3 represent count this function calculates position based on its count
        //                 1
        //              2     3
        //            4   5  6  7
        int height = log2(count) + 1;

        int xOffset = count - pow(2, height - 1);
        int x = initialOffsetAT(height) + xOffset * gapAtHeight(height);
        int y = height * (VerticalGap + rectHeight);
        return Point(x, y);
    }
    void draw(AVL& avl, SDL_Renderer* renderer) {
        //if there is no item in the avl
        if (info::deleting)return;
        if (avl.root == nullptr or info::deleting)return;

        treeHeight = 5;
        if (!info::treeThreadActive)return;

        //for message display
        SDL_Rect rec;
        surfaceMessage = TTF_RenderText_Solid(font, std::to_string(info::currentInsertingItem).c_str(), color);
        message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        rec.x = 1280 - 500;
        rec.y = 500;
        rec.w = 200 - 10;
        rec.h = 50;
        SDL_RenderCopy(renderer, message, NULL, &rec);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(message);

        SDL_Rect rect;
        std::queue<AVLNode*>queue;
        queue.push(avl.root);


        //maximum amount of nodes possible in given height
        //we store all those in queeue also null ptr because nullptr helps us in determining the x position
        int maxCount = pow(2, treeHeight) - 1;
        //count statring from one to sync with getPost function
        int count = 1;
        while (count < maxCount) {
            //to avoid deleted pointer being referene
            if (info::deleting)return;
            auto node = queue.front();
            queue.pop();

            if (node == nullptr) {
                queue.push(nullptr);
                queue.push(nullptr);
            }
            else {
                auto p = getPos(count);
                getRectangle(rect, p.x, p.y, rectWidth, rectHeight);

                try {
                    surfaceMessage = TTF_RenderText_Solid(font, std::to_string(node->data).c_str(), color);
                }
                catch (std::exception e) {
                    printf("exception thrown");
                    return;
                }
                message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

                if (node == info::avlNode) {
                    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

                }
                SDL_RenderFillCircle(renderer, rect.x + rectWidth / 2, rect.y + rectHeight / 2, rectHeight / 2 + 5);
                SDL_RenderCopy(renderer, message, NULL, &rect);
                //free surface and textr
                SDL_FreeSurface(surfaceMessage);
                SDL_DestroyTexture(message);


                queue.push(node->left);
                queue.push(node->right);

                if (node == info::avlNode) SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

                //this code is for drawing the line based on the count of the node
                //left to right (/)
                if (count % 2 == 0) {
                    Point from = Point(p.x + rectWidth, p.y);
                    int countOfParent = count / 2;
                    Point ParentPos = getPos(countOfParent);
                    Point to = Point(ParentPos.x, ParentPos.y + rectHeight);
                    SDL_RenderDrawLine(renderer, from.x, from.y, to.x, to.y);
                }
                //to draw line from right to left (\)
                else {
                    Point from = Point(p.x, p.y);
                    int countOfParent = count / 2;
                    if (countOfParent > 0) {
                        Point ParentPos = getPos(countOfParent);
                        Point to = Point(ParentPos.x + rectWidth, ParentPos.y + rectHeight);
                        SDL_RenderDrawLine(renderer, from.x, from.y, to.x, to.y);
                    }

                }
                SDL_SetRenderDrawColor(renderer, 200, 125, 0, 100);

            }
            count++;
        }
    };

};