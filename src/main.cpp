#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <array>
#include <cmath>
#include "shader_reader.hpp"

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

        -0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        0.8f, -0.8f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right
        0.0f, 0.8f, 0.0f,  0.0f, 0.0f, 1.0f, // up right
        -0.8f, 0.8f, 0.0f, 1.0f, 1.0f, 1.0f // up left
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

    Shader shader("../src/res/shaders/vertex_shader.glsl",
    "../src/res/shaders/fragment_shader.glsl");


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    while(!glfwWindowShouldClose(window))
    {
        // inputs
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);



        // float timeValue = glfwGetTime();
        // float greenValue = (sin(timeValue)/ 2.0f) + 0.5;
        // int vertexColorLocation = glGetUniformLocation(shader_program, "our_color");

        glBindVertexArray(vao);
        shader.use();
        // glUniform4f(vertexColorLocation, 0, greenValue, 0, 1.0f);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        
        
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}