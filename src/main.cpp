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
#include "mesh.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void display_matrix(const glm::mat4 mat)
{
    std::cout << "display matrix\n";
    for (int i = 0; i < 4; ++i)
    {
        std::cout << "[";
        for (int j = 0; j < 3; ++j)
        {
            std::cout << mat[j][i] << ", ";
        }
        std::cout << mat[3][i] << "]\n";
    }
}

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

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw_angle = -90.0f;
float pitch_angle = 0.0f;


glm::vec3 direction;

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
   
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    const float camera_speed = 0.1;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // front
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_pos += camera_speed * camera_front;
    }
    // back
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_pos -= camera_speed * camera_front;
    }

    // right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_pos += camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
    }
    // left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_pos += camera_speed * glm::normalize(glm::cross(camera_up, camera_front));
    }

    // up 
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        camera_pos += camera_speed * camera_up;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        camera_pos -= camera_speed * camera_up;
    }

    // right rotation
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        yaw_angle += glm::radians(1.0f);
        camera_front = glm::normalize(glm::vec3(
            sin(yaw_angle) * cos(pitch_angle),
            sin(pitch_angle),
            -cos(pitch_angle) * cos(yaw_angle)
        ));
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        yaw_angle -= glm::radians(1.0f);
        camera_front = glm::normalize(glm::vec3(
            sin(yaw_angle) * cos(pitch_angle),
            sin(pitch_angle),
            -cos(pitch_angle) * cos(yaw_angle)
        ));
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        pitch_angle += glm::radians(1.0f);
        if (pitch_angle > 89.0f)
        {
            pitch_angle = 89.0f;
        }
        camera_front = glm::vec3(
            sin(yaw_angle) * cos(pitch_angle),
            sin(pitch_angle),
            -cos(pitch_angle) * cos(yaw_angle)
        );
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        pitch_angle -= glm::radians(1.0f);
        if (pitch_angle < -89.0f)
        {
            pitch_angle = -89.0f;
        }
        camera_front = glm::vec3(
            sin(yaw_angle) * cos(pitch_angle),
            sin(pitch_angle),
            -cos(pitch_angle) * cos(yaw_angle)
        );
    }

}

int main(int argc, char const *argv[])
{
    yaw_angle = 0.0f;
    pitch_angle = 0.0f;
    if(!glfwInit()) {
        std::cerr << "glfw init failed\n";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "learnopengl", NULL, NULL);
    if (window == NULL) {
        std::cerr << "failed to create glfw window\n";
        return 2;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glfwSetCursorPosCallback(window, mouse_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // telling the opengl the size of our window
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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

    Model backpack("../src/res/models/backpack.obj");

    Shader shader{
        "../src/res/shaders/cube_vertex_shader.glsl",
        "../src/res/shaders/cube_fragment_shader.glsl"};


    glm::mat4 model = glm::mat4(1.0f); // identity matrix
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));

    // const float angle = -45.0f;
    // const float camera_distance = 4.0f;

    // glm::mat4 view = glm::lookAt(
    //     glm::vec3(0.0f, camera_distance*sin(glm::radians(angle)), camera_distance * cos(glm::radians(angle))),
    //     glm::vec3(0.0f, 0.0f, 0.0f),
    //     glm::vec3(0.0f, cos(glm::radians(angle)), -sin(glm::radians(angle)))
    // );

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);

    shader.use();
    unsigned int u_view_loc = glGetUniformLocation(shader.get_id(), "view");
    unsigned int u_model_loc = glGetUniformLocation(shader.get_id(), "model");
    unsigned int u_projection_loc = glGetUniformLocation(shader.get_id(), "projection");

    // glUniformMatrix4fv(u_view_loc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(u_model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(u_projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    while(!glfwWindowShouldClose(window))
    {
        // inputs

        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 view = glm::lookAt(
            camera_pos,
            camera_pos + camera_front,
            camera_up
        );
        glUniformMatrix4fv(u_view_loc, 1, GL_FALSE, glm::value_ptr(view));

        shader.use();
        backpack.draw(shader);

        // events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
