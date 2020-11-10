#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <queue>
#include <cmath>

#include "info.h"
#include "utils.h"
#include "bst.h"

struct Point {
    int x;
    int y;
    Point(int x, int y) :x(x), y(y) {}
};

class BSTGraph {
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

    BSTGraph() {
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
        //calculate height
        //printf("count = %d", count);
        int height = log2(count)+1;
        
        int xOffset = count - pow(2, height - 1);
        int x = initialOffsetAT(height) + xOffset * gapAtHeight(height);
        int y = height * (VerticalGap + rectHeight);
        return Point(x, y);
    }
    //void moveAndAnimate(SDL_Rect& rect, Point p2) {

    //    int slope = (p2.y - rect.y) / (p2.x - rect.x);
    //    int c = rect.y - slope * rect.x;
    //    while (rect.x != p2.x) {
    //        printf("print move and animate");
    //        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //        if (rect.x < p2.x) {
    //            rect.x++;
    //        }else{
    //            rect.x--;
    //        }
    //        rect.y = slope * rect.x+ c;

    //    }
    //}
    void draw(BST& bst, SDL_Renderer* renderer) {
        //if there is no item in the bst
        if (bst.getCount() == 0)return;

        treeHeight = 5;
        if (!info::treeThreadActive)return;

        //for message display

        //SDL_Rect rect;
        //surfaceMessage = TTF_RenderText_Solid(font, info::displayMessage.c_str(), color);
        //message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        //rect.x = 1280 - 200;
        //rect.y = 0;
        //rect.w = 200-10;
        //rect.h = 50;
        //SDL_RenderCopy(renderer, message, NULL, &rect);
        //SDL_FreeSurface(surfaceMessage);
        //SDL_DestroyTexture(message);
        
        SDL_Rect rect;
        std::queue<Node*>queue;
        queue.push(bst.root);

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
                       
                if (node == info::currentNode) {
                    SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);

                }
                SDL_RenderFillCircle(renderer, rect.x + rectWidth / 2, rect.y + rectHeight / 2, rectHeight / 2 + 5);
                SDL_RenderCopy(renderer, message, NULL, &rect);
                //free surface and textr
                SDL_FreeSurface(surfaceMessage);
                SDL_DestroyTexture(message);


                queue.push(node->left);
                queue.push(node->right);

                if (node == info::currentNode) SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
                //left to right
                if (count % 2 == 0) {
                    Point from = Point(p.x + rectWidth, p.y);
                    int countOfParent = count / 2;
                    Point ParentPos= getPos(countOfParent);
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