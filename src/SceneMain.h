//
// Created by 二九 on 2025/10/19.
//

#pragma once
#ifndef SPACEFIGHTER_SCENEMAIN_H
#define SPACEFIGHTER_SCENEMAIN_H
#include<list>
#include<random>
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

    // 发射子弹
    void shootPlayer();

    //更新子弹
    void updatePlayerProjectiles(float deltaTime);

    //渲染子弹
    void renderPlayerProjectiles();

    //生成敌机
    void spawEnemy();

    //更新敌机
    void updateEnemies(float deltaTime);

    //渲染敌机
    void renderEnemies();

    // 渲染敌机子弹
    void renderEnemyProjectiles();

    //更新敌机子弹
    void updateEnemyProjectiles(float deltaTime);

    void shootEnemy(Enemy *enemy); //敌机发射子弹
    SDL_FPoint getDirection(Enemy *enemy); //判断子弹的方向

    //更新玩家状态
    void updatePlayer(float deltaTime);

    //更新敌机爆炸的效果
    void enemyExplode(Enemy *enemy);

private:
    Game &game;
    Player player;
    // 创建子弹模板
    ProjectilePlayer projectilePlayerTemplate;
    //存储活动子弹列表
    std::list<ProjectilePlayer *> projectilesPlayer;


    //随机数生成器
    std::mt19937 gen;
    //随机数分布器
    std::uniform_real_distribution<float> dis;
    //敌机模板
    Enemy enemyTemplate;
    //敌机列表
    std::list<Enemy *> enemies;

    //敌机子弹模版
    ProjectileEnemy projectileEnemyTemplate;
    //敌机子弹容器
    std::list<ProjectileEnemy *> projectilesEnemy;
    //玩家是否死亡
    bool isDead = false;
};

#endif //SPACEFIGHTER_SCENEMAIN_H