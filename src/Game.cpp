//
// Created by 二九 on 2025/10/19.
//

#include "Game.h"

#include "SceneMain.h"

Game::Game() {
}

Game::~Game() {
}

void Game::run() {
    while (isRunning) {
        SDL_Event event;
        handleEvent(&event);
        update();
        render();
    }
}

void Game::handleEvent(SDL_Event *event) {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            isRunning = false;
        }
    }
    currentScene->handleEvent(event);
}

void Game::update() {
    currentScene->update();
}

void Game::render() {
    SDL_RenderClear(renderer);
    currentScene->render();
    SDL_RenderPresent(renderer);
}

void Game::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize! SDL error: %s", SDL_GetError());
        isRunning = false;
        return;
    }
    // 创建窗口
    SDL_Window *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window could not be created! SDL_Error: %s", SDL_GetError());
        isRunning = false;
        return;
    }
    // 创建渲染器, -1表示使用第一个支持的渲染驱动，SDL_RENDERER_ACCELERATED表示使用硬件加速的渲染
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer could not be created! SDL_Error: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    //初始化 SDL_Image
    if (IMG_Init(IMG_INIT_PNG)!=IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_Init: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    currentScene = new SceneMain();
    currentScene->init();
}

void Game::clean() {
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene) {
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}
