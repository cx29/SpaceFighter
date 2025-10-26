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
    //每秒移动 200px
    int speed = 200;
    //射击的冷却时间
    Uint32 coolDown = 500;
    //上次射击的时间
    Uint32 lastShootTime = 0;
};

struct ProjectilePlayer {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 400;
};

#endif //SPACEFIGHTER_OBJECT_H
