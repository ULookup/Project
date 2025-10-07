//#include "Game.h"
//#include <iostream>
//
//Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), speed(5.0f) {
//    player = { 375, 275, 50, 50 };  // ��ʼ�ڴ�������
//}
//
//Game::~Game() {
//    Clean();
//}
//
//bool Game::Init(const std::string& title, int width, int height)
//{
//    // ��ʼ�� SDL
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        std::cerr << "SDL ��ʼ��ʧ��: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    // ��������
//    window = SDL_CreateWindow(title.c_str(),
//        width, height,
//        SDL_WINDOW_RESIZABLE);
//    if (!window) {
//        std::cerr << "���ڴ���ʧ��: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    // ������Ⱦ��
//    renderer = SDL_CreateRenderer(window, nullptr);
//    if (!renderer) {
//        std::cerr << "��Ⱦ������ʧ��: " << SDL_GetError() << std::endl;
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
////        // �¼���ѯ
////        while (SDL_PollEvent(&event)) {
////            if (event.type == SDL_EVENT_QUIT) {
////                isRunning = false;
////            }
////            else if (event.type == SDL_EVENT_KEY_DOWN &&
////                event.key.key == SDLK_ESCAPE){
////                isRunning = false; // �� ESC �˳�
////            }
////        }
////
////        // ���ñ�����ɫ (R, G, B, A)
////        SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
////        SDL_RenderClear(renderer);   // �����Ļ
////        SDL_RenderPresent(renderer); // ��ʾ
////    }
////}
//void Game::Run() {
//    SDL_Event event;
//    while (isRunning) {
//        // �����¼�
//        while (SDL_PollEvent(&event)) {
//            HandleEvents(event);
//        }
//
//        // �����߼�
//        Update();
//
//        // ��Ⱦ
//        Render();
//
//        SDL_Delay(16); // ����֡��Լ 60fps
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
//    //�߽���
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
//    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);  // �������
//    SDL_RenderClear(renderer);
//
//    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);   // ������ɫ
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
//    // ��ʼ�� SDL
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        std::cerr << "SDL ��ʼ��ʧ��: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    // ��������
//    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
//    if (!window) {
//        std::cerr << "���ڴ���ʧ��: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    // ������Ⱦ��
//    renderer = SDL_CreateRenderer(window, nullptr);
//    if (!renderer) {
//        std::cerr << "��Ⱦ������ʧ��: " << SDL_GetError() << std::endl;
//        return false;
//    }
//
//    // ����Ĭ��������ɫ����ɫ��
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//
//    // ���� BMP ͼƬ
//    //SDL_Surface* surface = SDL_LoadBMP("Assets/player.bmp");
//    //if (!surface) {
//    //    std::cerr << "ͼƬ����ʧ��: " << SDL_GetError() << std::endl;
//    //    return false;
//    //}
//
//    //Uint32 colorkey = SDL_MapRGB(surface->format, 0, 255, 0);
//    //SDL_SetColorKey(surface, SDL_TRUE, colorkey);
//
//    //// �� Surface ת�� Texture
//    //SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
//    //SDL_FreeSurface(surface);  // ���� Surface ���ͷ�
//
//    //// �� Surface ת��Ϊ GPU ����
//    //playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
//    //SDL_DestroySurface(surface);  // �ͷ� CPU �ڴ�
//    //if (!playerTexture) {
//    //    std::cerr << "������ʧ��: " << SDL_GetError() << std::endl;
//    //    return false;
//    //}
//
//    // ��ʼ����ɫλ�ã��������ģ�
//    playerRect.w = 64;  // ���
//    playerRect.h = 64;  // �߶�
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
    // ��ʼ�� SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL ��ʼ��ʧ��: " << SDL_GetError() << std::endl;
        return false;
    }

    // ��������
    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "���ڴ���ʧ��: " << SDL_GetError() << std::endl;
        return false;
    }

    // ������Ⱦ��
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "��Ⱦ������ʧ��: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    // ʹ�� SDL3_image ���� PNG �ز�
    playerTexture = IMG_LoadTexture(renderer, "Assets/player.png");
    if (!playerTexture) {
        std::cerr << "ͼƬ����ʧ��: " << SDL_GetError() << std::endl;
        return false;
    }

    // ��ʼ����ɫλ��
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

    SDL_Quit(); // SDL3_image ����Ҫ���� Quit
}
