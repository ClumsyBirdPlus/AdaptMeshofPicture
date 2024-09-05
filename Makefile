# 编译器
CXX = mpicxx

# 编译选项
CXXFLAGS = -O2 -g `pkg-config --cflags --libs opencv4`

# 预处理选项
CPPFLAGS = -I$(HOME)/.local/include

# 链接器选项
LDFLAGS = -L$(HOME)/.local/lib

# 使用的库
LIBS = -lAFEPack `pkg-config --libs opencv4`

# 可执行文件名
TARGET = main

# 查找当前目录下所有的 .cpp 文件
SRCS = $(wildcard *.cpp)

# 将 .cpp 文件转换为对应的 .o 文件
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 链接目标
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# 编译每个 .cpp 文件为 .o 文件
%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: all
	-easymesh Coarse
	./run.sh

mesh:
	-easymesh Coarse

# 清理生成的文件
clean:
	rm -f *.o $(TARGET) *~ D.[nse]

# 伪目标
.PHONY: all clean $(TARGET)