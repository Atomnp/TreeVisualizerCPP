#pragma once

#include <iostream>
#include <random>    
#include <chrono>
#include <vector>
#include <string>
#include <SDL.h>

void getRectangle(SDL_Rect& r, int x, int y, int width, int height) {
    r.x = x;
    r.y = y;
    r.w = width;
    r.h = height;
}

std::vector<int> generateRandomArray(int n, int positiveUpto = 100) {
    std::random_device r;
    std::seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
    std::mt19937 eng(seed);

    std::uniform_int_distribution<> dist{ 1,positiveUpto };
    std::vector<int>result;
    for (int i = 0; i < n; ++i) {
        result.push_back(dist(eng) % positiveUpto);
    }
    return std::move(result);
}
void printArray(const std::vector<int>& arr) {
    std::string message = "[";
    for (int i = 0; i < arr.size(); i++) {
        message += std::to_string(arr[i]);
        message.push_back(',');
    }
    message.pop_back();
    message += "]\n";
    std::cout << message;

}
void swap(std::vector<int>& arr, int i, int j) {
    if (i == j)return;
    int temp = arr[j];
    arr[j] = arr[i];
    arr[i] = temp;
}
void swap(int* arr, int i, int j) {
    if (i == j)return;
    int temp = arr[j];
    arr[j] = arr[i];
    arr[i] = temp;
}
int SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


int
SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
            x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
            x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
            x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
            x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}