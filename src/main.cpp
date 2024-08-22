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

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    constexpr float camera_speed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_pos += camera_speed * camera_front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_pos -= camera_speed * camera_front;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_pos += camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_pos += camera_speed * glm::normalize(glm::cross(camera_up, camera_front));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera_pos += camera_up * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera_pos -= camera_up * camera_speed;
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
    glEnable(GL_DEPTH_TEST);


    std::array cube_vertices {
        // face 1 facing one
        // triangle 1
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
        // triangle 2
        0.5f, 0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,


        // face 2 // backwards one
        // triangle 1
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f,   0.5f, 1.0f, 0.0f, 0.0f,
        // triangle 2
        0.5f, 0.5f,   0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,


        // face 3
        -0.5f,  0.5f,  0.5f, 0.0f, 0.3f, 0.7f,
       -0.5f,  0.5f, -0.5f,  0.0f, 0.3f, 0.7f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.3f, 0.7f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.3f, 0.7f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.3f, 0.7f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.3f, 0.7f,
    
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,

       -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,

       -0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 0.5f,
        0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 0.5f,
        0.5f,  0.5f,  0.5f,  0.5f, 1.0f, 0.5f,
        0.5f,  0.5f,  0.5f,  0.5f, 1.0f, 0.5f,
       -0.5f,  0.5f,  0.5f,  0.5f, 1.0f, 0.5f,
       -0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 0.5f
    };

    unsigned int cube_vao;
    glGenVertexArrays(1, &cube_vao);
    glBindVertexArray(cube_vao);

    unsigned int cube_vbo;
    glGenBuffers(1, &cube_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(float), cube_vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));

    Shader cube_shader {
        "../src/res/shaders/cube_vertex_shader.glsl",
        "../src/res/shaders/cube_fragment_shader.glsl"
    };
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    



    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
    glm::mat4 view;
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

    cube_shader.use();
    unsigned int model_u_location = glGetUniformLocation(cube_shader.get_id(), "model");
    unsigned int view_u_location = glGetUniformLocation(cube_shader.get_id(), "view");
    unsigned int projection_u_location = glGetUniformLocation(cube_shader.get_id(), "projection");

    glUniformMatrix4fv(projection_u_location, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(model_u_location, 1, GL_FALSE, glm::value_ptr(model));
    


    while(!glfwWindowShouldClose(window))
    {
        // inputs
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(cube_vao);
        cube_shader.use();

        view = glm::lookAt(camera_pos, camera_front, camera_up);
        glUniformMatrix4fv(view_u_location, 1, GL_FALSE, glm::value_ptr(view));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}