#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <array>
#include <cmath>
#include "shader_reader.hpp"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char const *argv[])
{
    if(!glfwInit()) {
        std::cerr << "glfw init failed\n";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "learnopengl", NULL, NULL);
    if (window == NULL) {
        std::cerr << "failed to create glfw window\n";
        return 2;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // telling the opengl the size of our window
    glViewport(0, 0, 800, 600);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // vertex data
    std::array vertices{
        // -0.5f, -0.5f, 0.0f,
        // 0.5f, -0.5f, 0.0f,
        // 0.0f, 0.5f, 0.0f

        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // up right
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f // up left
    };

    // let's setup vertex array object
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // send data to vram
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    unsigned int indices[] = {
        0, 1, 2, // first triangle
        2, 3, 0 // second triangle
    };

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // at this point shaders are compiled and linked into shader_program

    // now data is at the vram and shaders are also, now we just need to define the layout
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));

    glEnableVertexAttribArray(2); // for texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    Shader shader("../src/res/shaders/vertex_shader.glsl",
    "../src/res/shaders/fragment_shader.glsl");

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // texture stuff
    // reading the data of the image
    int width, height, nrChannels;
    unsigned char* data = stbi_load("../src/res/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (!data)
        std::cout << "failed to load file 'texture'\n";

    // generate texture
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int texture_awesomeface;
    int w, h, nrc;
    glGenTextures(1, &texture_awesomeface);
    glBindTexture(GL_TEXTURE_2D, texture_awesomeface);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* awesome_data = stbi_load("../src/res/textures/awesomeface.png", &w, &h, &nrc, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, awesome_data);
    glGenerateMipmap(GL_TEXTURE_2D);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // maybe i need to set wrapping related stuff also

    stbi_image_free(data);
    stbi_image_free(awesome_data);

    glBindVertexArray(vao);
    shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_awesomeface);

    shader.set_int("texture1", 0);
    shader.set_int("texture2", 1);


    glm::mat4 trans = glm::mat4(1.0f);

    // trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // trans = glm::scale(trans, glm::vec3(0.5, 0.5f, 0.5f));

    unsigned int transformLoc = glGetUniformLocation(shader.get_id(), "transform");
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));




    while(!glfwWindowShouldClose(window))
    {
        // inputs
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);

        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        // trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

        trans = glm::scale(trans, glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}