#include "Model.h"
#include <stddef.h>
#include <string>
#include <glad/glad.h>
#include <stb_image.h>

std::vector<TexObj> textures_loaded;
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<TexObj> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    SetupMesh();
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VEO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(Program program) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        // Get texture number
        std::string number;
        std::string name = textures[i].type;

        if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
        if (name == "texture_specular") number = std::to_string(specularNr++);
        program.SetUniform1I(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

Model::Model(char *path) {
    LoadModel(path);
}

void Model::Draw(Program program) {
    // std::cout << "#meshes:" << meshes.size() << std::endl;
    for (unsigned int i = 0; i < meshes.size(); ++i) {
        //   std::cout << i << "th mesh : #vertices, #indices, #textures::"
        //<< meshes[i].vertices.size() << "," << meshes[i].indices.size() << ","
        //<< meshes[i].textures.size() << std::endl;
        meshes[i].Draw(program);
    }
}

void Model::LoadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << "Error: Assimp::" << importer.GetErrorString()
                  << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
    // 处理节点中所有的网格
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TexObj> textures;

    /*处理顶点*/
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                    mesh->mVertices[i].z);
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                  mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                         mesh->mTextureCoords[0][i].y);
            vertices.push_back(vertex);
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
    }

    /*处理索引*/
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    /*处理纹理*/
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<TexObj> diffuseMaps = LoadMaterialTextures(
            material, aiTextureType_DIFFUSE, "texture_diffuse");
        std::cout << "#diffuseMaps:" << diffuseMaps.size() << std::endl;
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<TexObj> specularMaps = LoadMaterialTextures(
            material, aiTextureType_SPECULAR, "texture_specular");
        std::cout << "#specularMaps:" << diffuseMaps.size() << std::endl;
        textures.insert(textures.end(), specularMaps.begin(),
                        specularMaps.end());

        std::vector<TexObj> heightMaps = LoadMaterialTextures(
            material, aiTextureType_HEIGHT, "texture_height");
        std::cout << "#heightMaps:" << diffuseMaps.size() << std::endl;
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        std::vector<TexObj> normalMaps = LoadMaterialTextures(
            material, aiTextureType_NORMALS, "texture_normal");
        std::cout << "#normalMaps:" << diffuseMaps.size() << std::endl;
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    }

    return {vertices, indices, textures};
}

std::vector<TexObj> Model::LoadMaterialTextures(aiMaterial *material,
                                                aiTextureType type,
                                                std::string typeName) {
    std::vector<TexObj> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
        aiString str;
        material->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); ++j) {
            // 如果该纹理已经被加载了，那么直接将该纹理添加

            if (std::strcmp(str.C_Str(), textures_loaded[j].path.C_Str())) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            TexObj texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char *textureName,
                             const std::string directory) {
    std::string filename = std::string(textureName);
    filename = directory + '/' + filename;
    unsigned int textureID;

    glGenTextures(1, &textureID);
    int width, height, nrComponents;

    unsigned char *data =
        stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1) {
            format = GL_RED;
        } else if (nrComponents == 3) {
            format = GL_RGB;
        } else if (nrComponents == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "TexObj :" << textureName << " loaded failed!";
        stbi_image_free(data);
    }

    return textureID;
}
