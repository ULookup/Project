#include <icelog/Logger.h>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    using namespace icelog;

    std::cout << "===== IceLog 黑箱测试开始 =====" << std::endl;

    // 1️⃣ 启用控制台日志输出
    EnableConsoleSink();
    LOG_INFO << "控制台日志输出已启用";

    // 2️⃣ 启用文件日志输出
    EnableFileSink();
    LOG_INFO << "文件日志输出已启用";

    // 3️⃣ 输出不同级别日志
    LOG_DEBUG   << "这是 DEBUG 级别日志";
    LOG_INFO    << "这是 INFO 级别日志";
    LOG_WARNING << "这是 WARNING 级别日志";
    LOG_ERROR   << "这是 ERROR 级别日志";
    LOG_FATAL   << "这是 FATAL 级别日志";

    // 4️⃣ 测试日志追加写入
    LOG_INFO << "再次写入文件日志（应当追加，不覆盖）";

    // 5️⃣ 测试多线程安全性
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            LOG_INFO << "来自线程 " << i << " 的日志";
        });
    }
    for (auto& t : threads) t.join();

    std::cout << "===== IceLog 黑箱测试完成 =====" << std::endl;
    std::cout << "请检查控制台输出和 logs/test.log 文件内容" << std::endl;
    return 0;
}
