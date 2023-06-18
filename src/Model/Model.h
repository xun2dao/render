#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../Shader/Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        static unsigned int GetCount() {
            return 8;
        }
};

struct TexObj {
        unsigned int id;
        std::string type;
        aiString path;
};

extern std::vector<TexObj> textures_loaded;

class Mesh {
    public:
        /* 网格数据*/
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<TexObj> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
             std::vector<TexObj> textures);

        void Draw(Program program);

    private:
        /*渲染数据*/
        unsigned int VAO, VBO, VEO;
        void SetupMesh();
};

class Model {
    public:
        Model(char *path);
        void Draw(Program);

    private:
        /*模型数据*/
        std::vector<Mesh> meshes;
        std::string directory;

        void LoadModel(std::string path);
        void ProcessNode(aiNode *node, const aiScene *scene);
        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<TexObj> LoadMaterialTextures(aiMaterial *mat,
                                                 aiTextureType type,
                                                 std::string typeName);
};

unsigned int TextureFromFile(const char *textureName, std::string directory);
