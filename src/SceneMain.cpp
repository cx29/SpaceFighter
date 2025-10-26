//
// Created by 二九 on 2025/10/19.
//

#include "SceneMain.h"

#include "Game.h"

SceneMain::SceneMain() : game(Game::getInstance()) {
}

SceneMain::~SceneMain() {
}

void SceneMain::init() {
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    if (player.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load player texture:%s", SDL_GetError());
    }
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;
}

void SceneMain::update(float deltaTime) {
    keyboardControl(deltaTime);
}

void SceneMain::render() {
    SDL_Rect playerRect = {
        static_cast<int>(player.position.x),
        static_cast<int>(player.position.y),
        player.width,
        player.height
    };
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::clean() {
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }
}

void SceneMain::handleEvent(SDL_Event *event) {
}

void SceneMain::keyboardControl(float deltaTime) {
    // 返回一个指向表示当前键盘状态的数组的指针，数组中每一个元素对应一个键位， 如果该键被按下则对应的元素值为 1，否则为 0
    auto keyboardState = SDL_GetKeyboardState(NULL);
    //使用的是 SCANCODE 扫描码,与物理位置有关，不是SDLK 更适合键盘控制
    if (keyboardState[SDL_SCANCODE_W]) {
        player.position.y -= deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        player.position.y += deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        player.position.x -= deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        player.position.x += deltaTime * player.speed;
    }
    if (player.position.x < 0) {
        player.position.x = 0;
    }
    if (player.position.x > game.getWindowWidth() - player.width) {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if (player.position.y < 0) {
        player.position.y = 0;
    }
    if (player.position.y > game.getWindowHeight() - player.height) {
        player.position.y = game.getWindowHeight() - player.height;
    }
}