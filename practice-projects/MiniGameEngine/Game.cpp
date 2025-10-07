//#include "Game.h"
//#include <iostream>
//
//Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), speed(5.0f) {
//    player = { 375, 275, 50, 50 };  // 初始在窗口中心
//}
//
//Game::~Game() {
//    Clean();
//}
//
//bool Game::Init(const std::string& title, int width, int height)
//{
//    // 初始化 SDL
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        std::cerr << "SDL 初始化失败: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    // 创建窗口
//    window = SDL_CreateWindow(title.c_str(),
//        width, height,
//        SDL_WINDOW_RESIZABLE);
//    if (!window) {
//        std::cerr << "窗口创建失败: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    // 创建渲染器
//    renderer = SDL_CreateRenderer(window, nullptr);
//    if (!renderer) {
//        std::cerr << "渲染器创建失败: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    isRunning = true;
//    return true;
//}
//
////void Game::Run()
////{
////    SDL_Event event;
////    while (isRunning) {
////        // 事件轮询
////        while (SDL_PollEvent(&event)) {
////            if (event.type == SDL_EVENT_QUIT) {
////                isRunning = false;
////            }
////            else if (event.type == SDL_EVENT_KEY_DOWN &&
////                event.key.key == SDLK_ESCAPE){
////                isRunning = false; // 按 ESC 退出
////            }
////        }
////
////        // 设置背景颜色 (R, G, B, A)
////        SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
////        SDL_RenderClear(renderer);   // 清空屏幕
////        SDL_RenderPresent(renderer); // 显示
////    }
////}
//void Game::Run() {
//    SDL_Event event;
//    while (isRunning) {
//        // 处理事件
//        while (SDL_PollEvent(&event)) {
//            HandleEvents(event);
//        }
//
//        // 更新逻辑
//        Update();
//
//        // 渲染
//        Render();
//
//        SDL_Delay(16); // 控制帧率约 60fps
//    }
//}
//
//void Game::HandleEvents(SDL_Event& event) {
//    if (event.type == SDL_EVENT_QUIT) {
//        isRunning = false;
//    }
//    else if (event.type == SDL_EVENT_KEY_DOWN) {
//        switch (event.key.key) {
//        case SDLK_ESCAPE:
//            isRunning = false;
//            break;
//        case SDLK_UP:
//            player.y -= speed;
//            break;
//        case SDLK_DOWN:
//            player.y += speed;
//            break;
//        case SDLK_LEFT:
//            player.x -= speed;
//            break;
//        case SDLK_RIGHT:
//            player.x += speed;
//            break;
//        default:
//            break;
//        }
//    }
//}
//
//void Game::Update() {
//    //边界检测
//    if (player.x < 0) player.x = 0;
//    if (player.y < 0) player.y = 0;
//
//    int w, h;
//    SDL_GetWindowSize(window, &w, &h);
//    if (player.x + player.w > w) player.x = w - player.w;
//    if (player.y + player.h > h) player.y = h - player.h;
//}
//
//void Game::Render() {
//    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);  // 背景深灰
//    SDL_RenderClear(renderer);
//
//    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);   // 方块绿色
//    SDL_RenderFillRect(renderer, &player);
//
//    SDL_RenderPresent(renderer);
//}
//
////void Game::Cleanup()
////{
////    if (renderer) {
////        SDL_DestroyRenderer(renderer);
////        renderer = nullptr;
////    }
////
////    if (window) {
////        SDL_DestroyWindow(window);
////        window = nullptr;
////    }
////
////    SDL_Quit();
////}
//
//void Game::Clean() {
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//}

//bool Game::Init(const char* title, int width, int height) {
//    // 初始化 SDL
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        std::cerr << "SDL 初始化失败: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    // 创建窗口
//    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
//    if (!window) {
//        std::cerr << "窗口创建失败: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    // 创建渲染器
//    renderer = SDL_CreateRenderer(window, nullptr);
//    if (!renderer) {
//        std::cerr << "渲染器创建失败: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    // 设置默认清屏颜色（黑色）
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//
//    // 加载 BMP 图片
//    //SDL_Surface* surface = SDL_LoadBMP("Assets/player.bmp");
//    //if (!surface) {
//    //    std::cerr << "图片加载失败: " << SDL_GetError() << std::endl;
//    //    return false;
//    //}
//
//    //Uint32 colorkey = SDL_MapRGB(surface->format, 0, 255, 0);
//    //SDL_SetColorKey(surface, SDL_TRUE, colorkey);
//
//    //// 把 Surface 转成 Texture
//    //SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
//    //SDL_FreeSurface(surface);  // 用完 Surface 就释放
//
//    //// 把 Surface 转换为 GPU 纹理
//    //playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
//    //SDL_DestroySurface(surface);  // 释放 CPU 内存
//    //if (!playerTexture) {
//    //    std::cerr << "纹理创建失败: " << SDL_GetError() << std::endl;
//    //    return false;
//    //}
//
//    // 初始化角色位置（窗口中心）
//    playerRect.w = 64;  // 宽度
//    playerRect.h = 64;  // 高度
//    playerRect.x = (width - playerRect.w) / 2;
//    playerRect.y = (height - playerRect.h) / 2;
//
//    isRunning = true;
//    return true;
//}
#include "Game.h"
#include <iostream>

Game::Game()
    : window(nullptr), renderer(nullptr), playerTexture(nullptr),
    isRunning(false) {
    playerRect = { 0, 0, 64, 64 };
}

Game::~Game() {
    Clean();
}

bool Game::Init(const char* title, int width, int height) {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL 初始化失败: " << SDL_GetError() << std::endl;
        return false;
    }

    // 创建窗口
    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "窗口创建失败: " << SDL_GetError() << std::endl;
        return false;
    }

    // 创建渲染器
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "渲染器创建失败: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    // 使用 SDL3_image 加载 PNG 素材
    playerTexture = IMG_LoadTexture(renderer, "Assets/player.png");
    if (!playerTexture) {
        std::cerr << "图片加载失败: " << SDL_GetError() << std::endl;
        return false;
    }

    // 初始化角色位置
    playerRect.w = 128;
    playerRect.h = 128;
    playerRect.x = (width - playerRect.w) / 2;
    playerRect.y = (height - playerRect.h) / 2;

    isRunning = true;
    return true;
}

void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            isRunning = false;
        }
        else if (event.type == SDL_EVENT_KEY_DOWN) {
            switch (event.key.key) {
            case SDLK_ESCAPE:
                isRunning = false;
                break;
            case SDLK_UP:
                playerRect.y -= 10;
                break;
            case SDLK_DOWN:
                playerRect.y += 10;
                break;
            case SDLK_LEFT:
                playerRect.x -= 10;
                break;
            case SDLK_RIGHT:
                playerRect.x += 10;
                break;
            default:
                break;
            }
        }
    }
}

void Game::Update() {
    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    if (playerRect.x < 0) playerRect.x = 0;
    if (playerRect.y < 0) playerRect.y = 0;

    if (playerRect.x + playerRect.w > winW)
        playerRect.x = winW - playerRect.w;
    if (playerRect.y + playerRect.h > winH)
        playerRect.y = winH - playerRect.h;
}

void Game::Render() {
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, playerTexture, nullptr, &playerRect);
    SDL_RenderPresent(renderer);
}

void Game::Clean() {
    if (playerTexture) {
        SDL_DestroyTexture(playerTexture);
        playerTexture = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit(); // SDL3_image 不需要单独 Quit
}
