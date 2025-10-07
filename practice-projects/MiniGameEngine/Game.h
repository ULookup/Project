//#pragma once
//#include <SDL3/SDL.h>
//#include <string>
//
//class Game {
//public:
//    Game();
//    ~Game();
//
//    bool Init(const std::string& title, int width, int height); // 初始化SDL
//    void Run();      // 游戏主循环
//    void Clean();  // 清理资源
//
//private:
//    SDL_Window* window;       // 窗口
//    SDL_Renderer* renderer;   // 渲染器
//    bool isRunning;           // 游戏循环状态
//
//    //方块属性
//    SDL_FRect player;
//    float speed;
//
//    void HandleEvents(SDL_Event& event);
//    void Update();
//    void Render();
//};
#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>

class Game {
public:
    Game();
    ~Game();

    bool Init(const char* title, int width, int height);  // 初始化窗口、渲染器和素材
    void HandleEvents();      // 处理事件
    void Update();            // 更新逻辑
    void Render();            // 渲染画面
    void Clean();             // 清理资源
    bool Running() const { return isRunning; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* playerTexture;     // 玩家纹理
    SDL_FRect playerRect;            // 玩家位置和大小

    bool isRunning;
};
