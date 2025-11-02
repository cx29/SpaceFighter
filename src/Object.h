//
// Created by 二九 on 2025/10/19.
//

#pragma once
#ifndef SPACEFIGHTER_OBJECT_H
#define SPACEFIGHTER_OBJECT_H
#include<SDL.h>

/// 玩家
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

    int currentHealth = 3;
};

/// 子弹
struct ProjectilePlayer {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 400;
    int damage = 1;
};

///敌人
struct Enemy {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200;
    Uint32 coolDown = 1000; //发射冷却时间
    Uint32 lastShootTime = 0; // 上次发射时间
    int currentHealth = 2;
};

// 敌人子弹
// 1. 需要根据自己位置和玩家位置来决定子弹的方向
struct ProjectileEnemy {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    //子弹方向， 归一化的二维向量
    SDL_FPoint direction = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 400;
    int damage = 1;
};

#endif //SPACEFIGHTER_OBJECT_H