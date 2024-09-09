#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "shader_reader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define POS_ATTRIB_INDEX 0
#define NORM_ATTRIB_INDEX 1
#define TEXC_ATTRIB_INDEX 2

struct vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;
};

struct texture {
    unsigned int id;
    std::string type;
};

class Mesh {
    public:
        std::vector<vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<texture> textures;

        Mesh(const std::vector<vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<texture>& textures);

        void draw(Shader& shader);

    private:
        unsigned int vao, vbo, ibo;

        void setup();
};

class Model {
    public:
        Model(const char* path) {load_model(path);}
        void draw(Shader& shader);
    private:
        std::vector<Mesh> meshes;
        std::string directory;

        void load_model(const std::string& path);
        void process_node(aiNode* node, const aiScene* scene);
        Mesh process_mesh(const aiMesh* mesh, const aiScene* scene);
};