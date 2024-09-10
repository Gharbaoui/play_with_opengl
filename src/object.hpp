#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "shader_reader.hpp"

#define SHADER_TEXTURE_UNIFORM_NAME "texture_1"

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texture_coords;
};

struct Texture {
    unsigned int id;
    std::string texture_path; // just for debugging later
};

struct Face {
    unsigned int start_index;
    unsigned int end_index;

    bool has_texture;
    unsigned int texture_index;
};

class   Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<Face>& faces,
        const std::vector<std::string>& texture_file_paths);
        void draw(Shader& shader);
    private:
        unsigned int vao, vbo, ebo;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indicies;
        std::vector<Face> faces;
        std::vector<Texture> textures;
        void setup(const std::vector<std::string>& texture_file_paths);
};