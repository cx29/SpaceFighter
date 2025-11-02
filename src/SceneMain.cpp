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
    //获取真随机数作为种子
    std::random_device rd;
    //用种子初始化梅森旋转引擎
    gen = std::mt19937(rd());
    //设置[0,1]均匀分布
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    //初始化玩家飞机
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    if (player.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load player texture:%s", SDL_GetError());
    }
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    //初始化玩家子弹模板
    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
    SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width /= 4;
    projectilePlayerTemplate.height /= 4;

    //初始化敌机模板
    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-2.png");
    SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= 4;
    enemyTemplate.height /= 4;

    //初始化敌机子弹模板
    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
    SDL_QueryTexture(projectileEnemyTemplate.texture, NULL, NULL, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width /= 4;
    projectileEnemyTemplate.height /= 4;
}

void SceneMain::update(float deltaTime) {
    keyboardControl(deltaTime);
    updatePlayerProjectiles(deltaTime); //更新玩家子弹
    updateEnemyProjectiles(deltaTime); //更新敌机子弹
    spawEnemy(); //生成敌机
    updateEnemies(deltaTime); //更新敌机
    updatePlayer(deltaTime); //更新玩家状态
}

void SceneMain::render() {
    //先渲染的会被后渲染的覆盖
    //渲染玩家子弹
    renderPlayerProjectiles();
    //渲染敌机子弹
    renderEnemyProjectiles();
    if (!isDead) {
        //渲染玩家
        SDL_Rect playerRect = {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height
        };
        SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
    }

    //渲染敌机
    renderEnemies();
}

void SceneMain::clean() {
    //清理敌机
    for (auto &enemy: enemies) {
        if (enemy != nullptr) {
            delete enemy;
        }
    }
    enemies.clear();
    //清理子弹
    for (auto &projectile: projectilesPlayer) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesPlayer.clear();

    //清理敌机子弹
    for (auto &projectile: projectilesEnemy) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesEnemy.clear();


    //清理玩家纹理
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }
    // 清除子弹纹理
    if (projectilePlayerTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }
    //清理敌机纹理
    if (enemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(enemyTemplate.texture);
    }
    //清理敌机子弹纹理
    if (projectileEnemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
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

    //射击逻辑
    if (keyboardState[SDL_SCANCODE_SPACE]) {
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime > player.coolDown) {
            shootPlayer();
            player.lastShootTime = currentTime;
        }
    }
}

void SceneMain::shootPlayer() {
    // 创建新子弹
    auto projectile = new ProjectilePlayer(projectilePlayerTemplate);
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;
    //添加到活动的子弹列表
    projectilesPlayer.push_back(projectile);
}

void SceneMain::updatePlayerProjectiles(float deltaTime) {
    //设定为子弹超出屏幕外边距的距离
    int margin = 32;
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();) {
        auto projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;
        if (projectile->position.y + margin < 0) {
            delete projectile;
            it = projectilesPlayer.erase(it);
        } else {
            bool hit = false;
            for (auto enemy: enemies) {
                SDL_Rect enemyRect = {
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height
                };
                SDL_Rect projectileRect = {
                    static_cast<int>(projectile->position.x),
                    static_cast<int>(projectile->position.y),
                    projectile->width,
                    projectile->height
                };
                if (SDL_HasIntersection(&enemyRect, &projectileRect)) {
                    enemy->currentHealth -= projectile->damage;
                    delete projectile;
                    it = projectilesPlayer.erase(it);
                    hit = true;
                    break;
                }
            }
            if (!hit) {
                ++it;
            }
        }
    }
}

void SceneMain::renderPlayerProjectiles() {
    for (auto projectile: projectilesPlayer) {
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
    }
}

void SceneMain::spawEnemy() {
    //利用随机数决定是否生成敌机（每帧约 1/60的概率）
    if (dis(gen) > 1 / 60.0f) {
        return;
    }
    Enemy *enemy = new Enemy(enemyTemplate);
    //利用随机数决定敌机的水平位置
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    //将敌机初始设置在屏幕外一个身位
    enemy->position.y = -enemy->height;
    enemies.push_back(enemy);
}

void SceneMain::updateEnemies(float deltaTime) {
    auto currentTime = SDL_GetTicks();

    for (auto it = enemies.begin(); it != enemies.end();) {
        auto enemy = *it;
        //更新敌机的位置
        enemy->position.y += enemy->speed * deltaTime;
        // 判定是否超出屏幕边缘位置，如果超出则需要进行资源释放
        if (enemy->position.y > game.getWindowHeight()) {
            delete enemy;
            it = enemies.erase(it);
        } else {
            //判断是否冷却结束可以设计
            if (currentTime - enemy->lastShootTime > enemy->coolDown) {
                shootEnemy(enemy);
                enemy->lastShootTime = currentTime;
            }
            if (enemy->currentHealth <= 0) {
                enemyExplode(enemy);
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void SceneMain::renderEnemies() {
    for (auto enemy: enemies) {
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
    }
}

void SceneMain::renderEnemyProjectiles() {
    for (auto projectile: projectilesEnemy) {
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        //ant2计算方向角, 返回从 X 轴正方向到向量（x，y）的弧度角
        //在 SDL 和 OpenGL 中旋转函数一般使用的是角度制（degree），需要将弧度*180/π转换为角度
        //子弹贴图一般是朝上， 减去 90度让旋转后的图像和实际运动方向对齐
        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI - 90;

        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL, &projectileRect, angle, NULL, SDL_FLIP_NONE);
    }
}

void SceneMain::updateEnemyProjectiles(float deltaTime) {
    auto margin = 32; //屏幕边缘额外的余量
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();) {
        auto projectile = *it;
        //根据方向向量更新子弹位置
        projectile->position.x += projectile->speed * projectile->direction.x * deltaTime;
        projectile->position.y += projectile->speed * projectile->direction.y * deltaTime;


        if (projectile->position.y > game.getWindowHeight() + margin ||
            projectile->position.y < -margin ||
            projectile->position.x < -margin ||
            projectile->position.x > game.getWindowWidth() + margin
        ) {
            delete projectile;
            it = projectilesEnemy.erase(it);
        } else {
            SDL_Rect projectileRect = {
                static_cast<int>(projectile->position.x),
                static_cast<int>(projectile->position.y),
                projectile->width,
                projectile->height
            };
            SDL_Rect playerRect = {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height
            };
            if (SDL_HasIntersection(&projectileRect, &playerRect) & !isDead) {
                player.currentHealth -= projectile->damage;
                delete projectile;
                it = projectilesEnemy.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void SceneMain::shootEnemy(Enemy *enemy) {
    //创建新的子弹
    auto projectile = new ProjectileEnemy(projectileEnemyTemplate);
    //设置子弹初始位置（从敌机的中心发射)
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;

    //计算子弹飞向玩家的位置
    projectile->direction = getDirection(enemy);
    projectilesEnemy.push_back(projectile);
}

//获取子弹方向
SDL_FPoint SceneMain::getDirection(Enemy *enemy) {
    //计算向量
    auto x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    auto y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);

    //计算向量长度
    auto length = sqrt(x * x + y * y);

    //进行归一化计算，防止向量模带来的计算复杂化
    x /= length;
    y /= length;
    return SDL_FPoint(x, y);
}

void SceneMain::updatePlayer(float deltaTime) {
    if (isDead) {
        return;
    }

    if (player.currentHealth <= 0) {
        isDead = true;
    }
    for (auto enemy: enemies) {
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_Rect playerRect = {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height
        };
        if (SDL_HasIntersection(&playerRect, &enemyRect)) {
            player.currentHealth -= 1;
            enemy->currentHealth = 0;
        }
    }
}

void SceneMain::enemyExplode(Enemy *enemy) {
    delete enemy;
}