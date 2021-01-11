#pragma once
// GL Includes
#include <glad/glad.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

namespace GEngine {

	class Model {
        bool gammaCorrection;
        public:
            Model(std::string path, bool convexHull = false){
                m_convexHull = convexHull;
                loadModel(path);
            }
            std::vector<Mesh*>* getMeshes() {
                return &meshes;
            }
            ~Model() {
                std::cout << "DELETED" << std::endl;
            }
            int getSize() {
                return this->meshes.size();
            }
            int getLoaded() {
                return loaded;
            }
          //  void draw(Shader& shader);
        private:
            // model data
            std::vector<Mesh*> meshes;
            std::string directory;
            std::vector<Texture*> textures_loaded;
            int loaded = 1;
            bool m_convexHull;
            void loadModel(std::string path);
            void processNode(aiNode* node, const aiScene* scene);
            Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
            std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                std::string typeName);





	};




}