cmake_minimum_required(VERSION 3.16)
project(IceLog LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ===============================
# 包含头文件目录
# ===============================
include_directories(${PROJECT_SOURCE_DIR}/include)

# ===============================
# 源文件列表
# ===============================
set(SRC_FILES
    src/Logger.cpp
    src/ConsoleSink.cpp
    src/FileSink.cpp
    src/LogLevel.cpp
    src/LogSink.cpp
    src/Mutex.cpp
    src/TimeUtil.cpp
)

# ===============================
# 生成静态库 icelog
# ===============================
add_library(icelog STATIC ${SRC_FILES})
target_include_directories(icelog PUBLIC ${PROJECT_SOURCE_DIR}/include)

# ===============================
# 编译测试程序
# ===============================
add_executable(test_logger tests/test_logger.cpp)
target_link_libraries(test_logger PRIVATE icelog)

# ===============================
# 编译选项（可选）
# ===============================
target_compile_options(icelog PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(test_logger PRIVATE -Wall -Wextra -Wpedantic)
