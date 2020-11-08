#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <queue>

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
    const int VerticalGap = 8;
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
    Point getPosition(int height, int xOffset) {

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

        SDL_Rect rect;
        surfaceMessage = TTF_RenderText_Solid(font, info::displayMessage.c_str(), color);
        message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        rect.x = 1280 - 200;
        rect.y = 0;
        rect.w = 200-10;
        rect.h = 50;
        SDL_RenderCopy(renderer, message, NULL, &rect);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(message);


        treeHeight = bst.treeHeight();
        if (treeHeight == 0 or !info::treeThreadActive)return;
       
        std::queue<Node*>queue;

        auto p = getPosition(1, 0);
        getRectangle(rect, p.x, p.y, rectWidth, rectHeight);
        if (info::currentNode == bst.root) {
            SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
        }
        surfaceMessage = TTF_RenderText_Solid(font, std::to_string(bst.root->data).c_str(), color);
        message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        SDL_RenderFillCircle(renderer, rect.x + rectWidth / 2, rect.y + rectHeight / 2, rectHeight / 2+5);
        SDL_RenderCopy(renderer, message, NULL, &rect);
        SDL_SetRenderDrawColor(renderer, 200, 125, 0, 100);
        //free surface and texture
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(message);

        queue.push(bst.root->left);
        queue.push(bst.root->right);
        int maxCount = pow(2, treeHeight) - 1;

        int prevHeight = 1;
        int xOffset = 1;
        int count = 1;
        while (count < maxCount) {
            auto node = queue.front();
            queue.pop();
            count++;
            if (node == nullptr) {
                if (xOffset >= pow(2, prevHeight - 1)) {
                    prevHeight++;
                    xOffset = 0;
                }
                xOffset++;
                queue.push(nullptr);
                queue.push(nullptr);
            }
            else {
                if (prevHeight != node->height) {
                    xOffset = 0;
                    prevHeight = node->height;
                }
                if (node->height < 0 or node->height>10) {
                    __debugbreak;
                }
                auto p = getPosition(node->height, xOffset);
                getRectangle(rect, p.x, p.y, rectWidth, rectHeight);
                try {
                    surfaceMessage = TTF_RenderText_Solid(font, std::to_string(node->data).c_str(), color);
                    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
                }
                catch(std::exception e) {
                    printf(e.what());
                }
               
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
                xOffset++;

                if (node == info::currentNode) SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
                if (count % 2 == 0) {
                    Point from = Point(p.x + rectWidth, p.y);
                    int parentX = from.x + (gapAtHeight(node->height) - rectWidth) / 2 - rectWidth / 2;
                    Point to = Point(parentX, p.y - VerticalGap);
                    SDL_RenderDrawLine(renderer, from.x, from.y, to.x, to.y);
                }
                else {
                    Point from = Point(p.x, p.y);
                    int parentX = from.x - (gapAtHeight(node->height) - rectWidth) / 2 + rectWidth / 2;
                    Point to = Point(parentX, p.y - VerticalGap);
                    SDL_RenderDrawLine(renderer, from.x, from.y, to.x, to.y);
                }
                SDL_SetRenderDrawColor(renderer, 200, 125, 0, 100);

            }
        }
    };

};