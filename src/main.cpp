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
            cos(yaw_angle) * cos(pitch_angle),
            sin(pitch_angle),
            cos(pitch_angle)*sin(yaw_angle)
        ));
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        yaw_angle -= glm::radians(1.0f);
        camera_front = glm::normalize(glm::vec3(
            cos(yaw_angle) * cos(pitch_angle),
            sin(pitch_angle),
            cos(pitch_angle)*sin(yaw_angle)
        ));
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        pitch_angle += glm::radians(1.0f);
        camera_front = glm::normalize(glm::vec3(
            cos(yaw_angle) * cos(pitch_angle),
            sin(pitch_angle),
            cos(pitch_angle)*sin(yaw_angle)
        ));
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        pitch_angle -= glm::radians(1.0f);
        camera_front = glm::normalize(glm::vec3(
            cos(yaw_angle) * cos(pitch_angle),
            sin(pitch_angle),
            cos(pitch_angle)*sin(yaw_angle)
        ));
    }
}

int main(int argc, char const *argv[])
{
    yaw_angle = -90.0f;
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

    constexpr std::array square_vertices{
        // x    y      z     R     G     B

        // front face
        -0.5f, -0.5f, 0.5f,  0.1f, 0.3f, 0.1f, 0.0f, 0.0f,     // 0
        0.5f, -0.5f, 0.5f,   0.4f, 0.1f, 0.7f, 1.0f, 0.0f,     // 1
        0.5f, 0.5f, 0.5f,    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,      // 2
        -0.5f, 0.5f, 0.5f,   0.3f, 0.5f, 0.0f, 0.0f, 1.0f,       // 3

        // right side
        0.5f, 0.5f, 0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f,      // 4
        0.5f, 0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,     // 5
        0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,     // 6
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f,   // 7
        
        // left side
        -0.5f, 0.5f, 0.5f,   1.0f,  0.5f, 0.0f,  0.0f, 1.0f,  // 8
        -0.5f, 0.5f, -0.5f,  1.0f,  0.5f, 0.0f,  1.0f, 1.0f,  // 9
        -0.5f, -0.5f, 0.5f,  1.0f,  0.5f, 0.0f,  0.0f, 0.0f,  // 10
        -0.5f, -0.5f, -0.5f, 1.0f,  0.5f, 0.0f,  1.0f, 0.0f,  // 11
        
        
        // bottom face
         0.5f, -0.5f, 0.5f,   .0f,  0.5f, 1.0f,  1.0f, 1.0f,   // 12
         0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 0.0f,  1.0f, 0.0f,   // 13
        -0.5f, -0.5f, 0.5f,   .0f,  0.5f, 1.0f,  0.0f, 1.0f,   // 14
        -0.5f, -0.5f, -0.5f,  .0f,  0.5f, 0.0f,  0.0f, 0.0f,   // 15
        
        // top face
         0.5f, 0.5f, 0.5f,   .1f,  0.5f, 0.0f,   1.0f, 1.0f,  // 16
         0.5f, 0.5f, -0.5f,  .0f,  0.0f, 0.0f,   1.0f, 0.0f,  // 17
        -0.5f, 0.5f, 0.5f,   .0f,  1.0f, 1.0f,   0.0f, 1.0f,  // 18
        -0.5f, 0.5f, -0.5f,  1.0f,  1.0f, 1.0f,  0.0f, 0.0f,  // 19


        // back face
        -0.5f, -0.5f, -0.5f, 0.3f, 0.1f, 0.7f,  0.0f, 0.0f,  // 20
        0.5f, -0.5f, -0.5f,  0.4f, 0.2f, 0.3f,  1.0f, 0.0f,  // 21
        0.5f, 0.5f, -0.5f,   0.5f, 0.1f, 0.5f,  1.0f, 1.0f,  // 22
        -0.5f, 0.5f, -0.5f,  0.3f, 0.5f, 0.8f,   0.0f, 1.0f  // 23
    };

    constexpr std::array square_indices{
        // front face
        0U, 1U, 2U,
        2U, 3U, 0U,

        // right face
        6U, 5U, 4U,
        6U, 7U, 5U,

        // left face
        10U, 9U, 8U,
        10U, 11U, 9U,

        // bottom face
        14U, 12U, 13U,
        13U, 15U, 14U,

        // top face
        18U, 16U, 17U,
        17U, 19U, 18U,

        // back face
        20U, 21U, 22U,
        22U, 23U, 20U
    };

    // create voa
    unsigned int square_vao;
    glGenVertexArrays(1, &square_vao);
    glBindVertexArray(square_vao);
    // create vbo
    unsigned int square_vbo;
    glGenBuffers(1, &square_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, square_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * square_vertices.size(), square_vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    unsigned int square_ibo;
    glGenBuffers(1, &square_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, square_indices.size() * sizeof(unsigned int), square_indices.data(), GL_STATIC_DRAW);

    Shader shader{
        "../src/res/shaders/cube_vertex_shader.glsl",
        "../src/res/shaders/cube_fragment_shader.glsl"};
    
    glUseProgram(0);
    
    // unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // textures
    int width, height, num_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* brick_wall_texture_data = stbi_load("../src/res/textures/brick.jpg", &width, &height, &num_channels, 0);
    if (!brick_wall_texture_data) {
        std::cout << "could not load texture stbi_load error\n";
    }


    unsigned int brick_wall_texture_obj;
    glGenTextures(1, &brick_wall_texture_obj);
    glBindTexture(GL_TEXTURE_2D, brick_wall_texture_obj);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, brick_wall_texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(brick_wall_texture_data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brick_wall_texture_obj);
    glUniform1i(glGetUniformLocation(shader.get_id(), "our_interface_to_texture_unit"), 0);



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

        glBindVertexArray(square_vao);
        shader.use();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_ibo);

        const glm::mat4 view = glm::lookAt(
            camera_pos,
            camera_pos + camera_front,
            camera_up
        );
        glUniformMatrix4fv(u_view_loc, 1, GL_FALSE, glm::value_ptr(view));

        glDrawElements(GL_TRIANGLES, square_indices.size(), GL_UNSIGNED_INT, NULL);

        // events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
