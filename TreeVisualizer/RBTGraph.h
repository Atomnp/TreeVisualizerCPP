#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <queue>
#include <cmath>
#include <string>


#include "info.h"
#include "utils.h"
#include "RedBlackTree.h"

struct Point {
    int x;
    int y;
    Point(int x, int y) :x(x), y(y) {}
};

class RBTGraph {
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

    RBTGraph() {
        TTF_Init();
        font = TTF_OpenFont("Sans.ttf", 24); //this opens a font style and sets a size
        color = { 255, 255, 255 };
    }

    //height from top or root
    int gapAtHeight(int h) {
        return lastGap * pow(2, treeHeight - h + 1);
    }
    int initialOffsetAT(int height) {

        if (height == treeHeight)return 0;
        if (treeHeight - height == 1) {
            return (rectWidth + (gapAtHeight(height + 1))) / 2 - rectWidth / 2;
        }
        return initialOffsetAT(height + 1) + (rectWidth + (gapAtHeight(height + 1))) / 2 - rectWidth / 2;
    }
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
    //void move(int count1, int count2) {
    //    Point p2 = getPos(count2);
    //    Point p1 = getPos(count1);
    //    int slope = (p2.y - p1.y) / (p2.x - p1.x);
    //    int c = p1.y - slope * p1.x;
    //    while (p1.x != p2.x) {
    //        printf("print move and animate");
    //        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //        if (p1.x < p2.x) {
    //            p1.x++;
    //        }
    //        else {
    //            p1.x--;
    //        }
    //        p1.y = slope * p1.x + c;
    //    }
    void draw(RedBlackTree& rbt, SDL_Renderer* renderer) {
        //if there is no item in the rbt
        if (rbt.root == nullptr or info::deleting)return;

        treeHeight = 5;
        if (!info::treeThreadActive)return;

        //for message display

        SDL_Rect rec;
        surfaceMessage = TTF_RenderText_Solid(font, std::to_string(info::currentInsertingItem).c_str(), color);
        message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        rec.x = 1280 - 500;
        rec.y = 500;
        rec.w = 200-10;
        rec.h = 50;
        SDL_RenderCopy(renderer, message, NULL, &rec);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(message);

        SDL_Rect rect;
        std::queue<RBTNode*>queue;
        queue.push(rbt.root);

        int maxCount = pow(2, treeHeight) - 1;
        int count = 1;
        while (count < maxCount) {
            auto node = queue.front();
            queue.pop();

            if (node == nullptr) {
                queue.push(nullptr);
                queue.push(nullptr);
            }
            else {
                auto p = getPos(count);
                getRectangle(rect, p.x, p.y, rectWidth, rectHeight);

                surfaceMessage = TTF_RenderText_Solid(font, std::to_string(node->data).c_str(), color);
                message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

                if (node->isRed) {
                    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
                }
                if (node == info::rbtnode) {
                    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

                }
                SDL_RenderFillCircle(renderer, rect.x + rectWidth / 2, rect.y + rectHeight / 2, rectHeight / 2 + 5);
                SDL_RenderCopy(renderer, message, NULL, &rect);
                //free surface and textr
                SDL_FreeSurface(surfaceMessage);
                SDL_DestroyTexture(message);


                queue.push(node->left);
                queue.push(node->right);

                if (node == info::rbtnode) SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                //if (node->isRed) SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
                //left to right
                if (count % 2 == 0) {
                    Point from = Point(p.x + rectWidth, p.y);
                    int countOfParent = count / 2;
                    Point ParentPos = getPos(countOfParent);
                    Point to = Point(ParentPos.x, ParentPos.y + rectHeight);
                    /* int parentX = from.x + (gapAtHeight(node->height) - rectWidth) / 2 - rectWidth / 2;*/
                     //Point to = Point(parentX, p.y - VerticalGap);
                    SDL_RenderDrawLine(renderer, from.x, from.y, to.x, to.y);
                }
                else {
                    Point from = Point(p.x, p.y);
                    int countOfParent = count / 2;
                    //not to draw line at the top of root node
                    if (countOfParent > 0) {
                        Point ParentPos = getPos(countOfParent);
                        Point to = Point(ParentPos.x + rectWidth, ParentPos.y + rectHeight);
                        /*  int parentX = from.x - (gapAtHeight(node->height) - rectWidth) / 2 + rectWidth / 2;
                          Point to = Point(parentX, p.y - VerticalGap);*/
                        SDL_RenderDrawLine(renderer, from.x, from.y, to.x, to.y);
                    }

                }
                SDL_SetRenderDrawColor(renderer, 200, 125, 0, 100);

            }
            count++;
        }
    };

};