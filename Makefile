GLFW_LIB_FOLDER = ./deps/glfw/build/src
GLFW_LIB_NAME = glfw3
GLFW_INCLUDE = ./deps/glfw/include

GLAD_INCLUDE = ./deps/glad/include
GLAD_SRC = ./deps/glad/src/glad.c

SRC_FILES = ./src/main.cpp ./src/shader_reader.cpp
OUT_FILE = ./build/main.elf

GENERATED_FILE = $(OUT_FILE)

all: $(folder_setup)
	g++ $(SRC_FILES) $(GLAD_SRC) -I$(GLFW_INCLUDE) -I$(GLAD_INCLUDE) -L$(GLFW_LIB_FOLDER) -l$(GLFW_LIB_NAME) -lGL -o $(OUT_FILE)


folder_setup:
	mkdir -p build

re: clean all

clean:
	rm -f $(GENERATED_FILE)


# For Linux users compiling with GCC, the following command line options may help
# youcompiletheproject:-lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl. Not correctly linking the corresponding libraries will generate many undefined
# reference errors.