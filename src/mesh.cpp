#include "mesh.hpp"

Mesh::Mesh(const std::vector<vertex>& vertices,
    const std::vector<unsigned int>& indices,
    const std::vector<texture>& textures)
{

    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setup();
}

void Mesh::setup()
{
    // VERTEX ARRAY OBJECT
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // VERTEX BUFFER OBJECT
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
    vertices.size() * sizeof(vertex),
    vertices.data(), GL_STATIC_DRAW);

    // ELEMENT/INDEX BUFFER OBJECT
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    indices.size() * sizeof(unsigned int),
    indices.data(), GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(POS_ATTRIB_INDEX);
    glVertexAttribPointer(POS_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);


    // normal attribute
    glEnableVertexAttribArray(NORM_ATTRIB_INDEX);
    glVertexAttribPointer(NORM_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE,
    sizeof(vertex), (void*)(offsetof(vertex, normal)));

    // texture coordinates
    glEnableVertexAttribArray(TEXC_ATTRIB_INDEX);
    glVertexAttribPointer(TEXC_ATTRIB_INDEX, 2, GL_FLOAT,
    GL_FALSE, sizeof(vertex), (void*)(offsetof(vertex, texture_coordinates)));


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(Shader& shader)
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void Model::draw(Shader& shader)
{
    for (auto& mesh: meshes) {
        mesh.draw(shader);
    }
}

void Model::load_model(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "error::assimp::" << importer.GetErrorString() << "\n";
        return;
    }

    directory = path.substr(0, path.find_last_of("/"));

    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        process_node(node->mChildren[i], scene);
    }
}


Mesh Model::process_mesh(const aiMesh* mesh, const aiScene* scene)
{
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        vertex tmp;

        // position
        tmp.position.x = mesh->mVertices[i].x;
        tmp.position.y = mesh->mVertices[i].y;
        tmp.position.z = mesh->mVertices[i].z;

        // texture coordinates
        if (mesh->mTextureCoords[0])
        {
            tmp.texture_coordinates.x = mesh->mTextureCoords[0][i].x;
            tmp.texture_coordinates.y = mesh->mTextureCoords[0][i].y;
        } else {
            tmp.texture_coordinates = glm::vec2(0, 0);
        }

        // normals not yet

        vertices.push_back(tmp);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh(vertices, indices, textures);
}