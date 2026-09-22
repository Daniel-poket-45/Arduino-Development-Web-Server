#Automated compiler for ADWS - Arduino Development Web Server

#############################
#  COMPILER CONFIG SECTION  #
#############################

TARGET = index.html
CXX = em++
CXXFLAGS = -Wall

INCLUDE_DIRS = -Iinclude
LIB_DIRS = -Llib
STATIC_LIBS = $(wildcard lib/*.a)
SRC = $(wildcard src/*.cpp)
LIB_OBJS = $(wildcard obj/*.o)

# Flags específicas do Emscripten para SDL3 e WebGL/OpenGL ES 3
EM_FLAGS = -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 -s MAX_WEBGL_VERSION=2 --shell-file template.html

all:
	@echo $(shell date): Compiling ADWS [webGL] at $(shell hostname) as $(shell whoami)
	$(CXX) $(CXXFLAGS) $(SRC) $(LIB_OBJS) $(STATIC_LIBS) -o $(TARGET) $(INCLUDE_DIRS) $(LIB_DIRS) $(EM_FLAGS)
	@echo ADWS [local-test] compiled! Executable name: $(TARGET)

clean:
	rm -f index.html index.js index.wasm
	@echo "Clean completed!"