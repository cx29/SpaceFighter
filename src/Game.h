//
// Created by 二九 on 2025/10/19.
//

#pragma once
#ifndef SPACEFIGHTER_GAME_H
#define SPACEFIGHTER_GAME_H
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include "Scene.h"

class Game {
public:
    // 线程安全，懒汉式单例模式
    static Game &getInstance() {
        static Game instance;
        return instance;
    }

    ~Game();

    //游戏主循环
    void run();

    //初始化游戏
    void init();

    //清理资源
    void clean();

    //切换场景
    void changeScene(Scene *scene);

    void handleEvent(SDL_Event *event);

    void update();

    void render();

    SDL_Window *getWindow() { return window; }
    SDL_Renderer *getRenderer() { return renderer; }
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }

private:
    Game();

    // 删除拷贝构造函数
    Game(const Game &) = delete;

    // 删除赋值构造函数
    Game &operator=(const Game &) = delete;

    bool isRunning = true;
    Scene *currentScene = nullptr;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;
};

#endif //SPACEFIGHTER_GAME_H
