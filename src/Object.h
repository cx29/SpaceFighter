//
// Created by 二九 on 2025/10/19.
//

#pragma once
#ifndef SPACEFIGHTER_OBJECT_H
#define SPACEFIGHTER_OBJECT_H
#include<SDL.h>

struct Player {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
};

#endif //SPACEFIGHTER_OBJECT_H
