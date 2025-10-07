//#include "Game.h"
//
//int main(int argc, char* argv[])
//{
//    Game game;
//
//    if (!game.Init("SDL3 Demo Window", 800, 600)) {
//        return -1;  // 初始化失败
//    }
//    else
//        game.Run();      // 进入主循环
//
//    game.Clean();  // 清理资源
//
//    return 0;
//}
#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Game game;

    if (!game.Init("Mini Game Engine - SDL3 + SDL_image", 800, 600)) {
        std::cerr << "游戏初始化失败！" << std::endl;
        return -1;
    }

    while (game.Running()) {
        game.HandleEvents();
        game.Update();
        game.Render();
        SDL_Delay(16); // ~60FPS
    }

    game.Clean();
    return 0;
}
