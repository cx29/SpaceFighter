//
// Created by 二九 on 2025/10/19.
//

#pragma once
#ifndef SPACEFIGHTER_SCENE_H
#define SPACEFIGHTER_SCENE_H
#include <SDL.h>

class Scene {
public:
    Scene() = default;

    virtual ~Scene() = default;

    //初始化场景
    virtual void init() =0;

    //更新场景状态
    virtual void update() =0;

    //渲染场景
    virtual void render() =0;

    //清理场景资源
    virtual void clean() =0;

    //处理输入事件
    virtual void handleEvent(SDL_Event *event) =0;
};
#endif //SPACEFIGHTER_SCENE_H
