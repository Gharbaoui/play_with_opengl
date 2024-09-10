#include "object.hpp"
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

Mesh::Mesh(const std::vector<Vertex>& vertices,
const std::vector<unsigned int>& indices,
const std::vector<Face>& faces,
const std::vector<std::string>& texture_file_paths)
{
    this->vertices = vertices;
    this->indicies = indices;
    this->faces = faces;
    textures.resize(texture_file_paths.size());

    setup(texture_file_paths);
}

void Mesh::setup(const std::vector<std::string>& texture_file_paths)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
    vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    glEnableVertexAttribArray(1); // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    (void*)(offsetof(Vertex, color)));

    glEnableVertexAttribArray(2); // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    (void*)(offsetof(Vertex, texture_coords)));

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    indicies.size() * sizeof(unsigned int), indicies.data(), GL_STATIC_DRAW);

    
    for (unsigned int i = 0; i < texture_file_paths.size(); ++i) {
        int width, height, num_channels;
        const std::string& texture_path = texture_file_paths[i];
        // stbi_set_flip_vertically_on_load(true);
        unsigned char* data =
        stbi_load(texture_path.c_str(), &width, &height, &num_channels, 0);
        

        if (!data) {
            std::cout << "could not load texture stbi_load error: " << texture_path.c_str() << "\n";
            exit(1);
        }
        unsigned int id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
        textures[i].texture_path = texture_path;
        textures[i].id = id;
        // now the data is there when we draw well active texture unit send to uniform that's it
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::draw(Shader& shader)
{
    shader.use();
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    unsigned int texture_un_id = glGetUniformLocation(shader.get_id(),
    SHADER_TEXTURE_UNIFORM_NAME);

    for (Face& face: faces)
    {
        if (face.has_texture)
        {
            glActiveTexture(GL_TEXTURE0); // try to move this outside
            glBindTexture(GL_TEXTURE_2D, textures[face.texture_index].id);
            // let's send it to the shader
            glUniform1i(texture_un_id, 0); // try to move this outside
        }

        glDrawElements(GL_TRIANGLES, (face.end_index - face.start_index + 1),
        GL_UNSIGNED_INT, (void*)(face.start_index * sizeof(unsigned int)));
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}