GLFW_LIB_FOLDER = ./deps/glfw/build/src
GLFW_LIB_NAME = glfw3
GLFW_INCLUDE = ./deps/glfw/include

GLAD_INCLUDE = ./deps/glad/include
GLAD_SRC = ./deps/glad/src/glad.c

STB_INCLUDE = ./src/vendor

SRC_FILES = ./src/main.cpp ./src/shader_reader.cpp ./src/vendor/stb_image_loader_impl.cpp ./src/mesh.cpp
OBJ_FILES = $(patsubst %.cpp, ./build/%.o, $(notdir $(SRC_FILES)))
OUT_FILE = ./build/main.elf

ASSIMP_LIB_NAME=assimp
ASSIMP_LIB_PATH=./deps/assimp/bin
ASSIMP_INCLUDE_PATH=./deps/assimp/include

GLM_PATH = ./deps/glm

GENERATED_FILE = $(OUT_FILE) $(OBJ_FILES)

all: $(OBJ_FILES)
	g++ $(OBJ_FILES) $(GLAD_SRC) -I$(GLFW_INCLUDE) -I$(GLAD_INCLUDE) \
	-I$(STB_INCLUDE) -I$(GLM_PATH) -L$(GLFW_LIB_FOLDER) -l$(GLFW_LIB_NAME) \
	-lGL -L$(ASSIMP_LIB_PATH) -l$(ASSIMP_LIB_NAME) -o $(OUT_FILE)

./build/%.o: ./src/%.cpp
	g++ -c -I$(ASSIMP_INCLUDE_PATH) -I$(GLFW_INCLUDE) -I$(GLAD_INCLUDE) -I$(STB_INCLUDE) -I$(GLM_PATH) -L$(GLFW_LIB_FOLDER) -l$(GLFW_LIB_NAME) -lGL $^ -o $@

./build/%.o: ./src/vendor/%.cpp
	g++ -c -I$(GLFW_INCLUDE) -I$(GLAD_INCLUDE) -I$(STB_INCLUDE) -I$(GLM_PATH) -L$(GLFW_LIB_FOLDER) -l$(GLFW_LIB_NAME) -lGL $^ -o $@

folder_setup:
	mkdir -p build

re: clean all

clean:
	rm -f $(GENERATED_FILE)


# For Linux users compiling with GCC, the following command line options may help
# youcompiletheproject:-lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl. Not correctly linking the corresponding libraries will generate many undefined
# reference errors.