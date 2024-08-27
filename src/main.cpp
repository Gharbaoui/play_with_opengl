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


void glDebugOutput(
    GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* msg,
    const void* userParam
)
{
    std::cout << "!!! Debug callback !!!\n";
    std::cout << "debug message: id " << id << ", " << msg << "\n";

    std::cout << "msg source: ";

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
            std::cout << "API" << "\n";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "window system\n";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "shader compiler\n";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "third party \n";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "application \n";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "other \n";
        break;
    default:
        break;
    }

    std::cout << "error type: ";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Error\n";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "DEPRECATED BEHAVIOR\n";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "UNDEFINED BEHAVIOR\n";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "PORTABILITY\n";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "PERFORMANCE\n";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "MARKER\n";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "PUSH_GROUP\n";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "POP_GROUP\n";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "OTHER\n";
        break;
    default:
        break;
    }
}

glm::vec3 camera_pos = glm::vec3(3.0f, 2.0f, 0.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

float delta_time = 0.0f;
float last_frame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    const float camera_speed = 2 * delta_time;
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
}

int main(int argc, char const *argv[])
{
    if(!glfwInit()) {
        std::cerr << "glfw init failed\n";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

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
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, NULL);
    glDebugMessageControl(
        GL_DONT_CARE,
        GL_DONT_CARE,
        GL_DONT_CARE,
        0, NULL, GL_TRUE
    );


    constexpr std::array cube_vertices {
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
    

    double yaw, pitch;
    yaw = 0;
    pitch = 0;

    const glm::vec3 direction{
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::sin(glm::radians(pitch)),
        glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
    };
    std::cout << "x: " << direction.x << " y: " << direction.y << " z: " << direction.z << std::endl;

    camera_front = glm::normalize(direction);

    while(!glfwWindowShouldClose(window))
    {
        // inputs
        const float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame; // so bigger rendering time bigger delta time
        last_frame = current_frame;
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