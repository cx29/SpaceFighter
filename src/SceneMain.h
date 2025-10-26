//
// Created by 二九 on 2025/10/19.
//

#pragma once
#ifndef SPACEFIGHTER_SCENEMAIN_H
#define SPACEFIGHTER_SCENEMAIN_H
#include "Scene.h"
#include"Object.h"

class Game;

class SceneMain : public Scene {
public:
    SceneMain();

    ~SceneMain();

    void init() override;

    void update(float deltaTime) override;

    void render() override;

    void clean() override;

    void handleEvent(SDL_Event *event) override;

    // 轮询方式查询键盘事件
    void keyboardControl(float deltaTime);

private:
    Game &game;
    Player player;
};

#endif //SPACEFIGHTER_SCENEMAIN_H