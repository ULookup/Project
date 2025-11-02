# ===== 基本设置 =====
CXX := g++
CXXFLAGS := -std=c++20 -Wall -O2 -I. -I./core -I./net -I./lib/IceLog
LDFLAGS := -pthread
TARGET := bin/Server

# ===== 自动收集所有 cpp 文件 =====
SRC := $(shell find core net test -name "*.cpp")
OBJ := $(SRC:.cpp=.o)

# ===== 默认目标 =====
all: $(TARGET)

# ===== 链接阶段 =====
$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS) -L./lib/IceLog -licelog -Wl,-rpath,'$$ORIGIN/../lib/IceLog'

# ===== 编译阶段 =====
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ===== 清理 =====
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
