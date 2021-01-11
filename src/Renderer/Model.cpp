#include "Model.h"



namespace GEngine {

	void Model::loadModel(std::string path) {
		// Read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		// Retrieve the directory path of the filepath
		this->directory = path.substr(0, path.find_last_of('/'));

		// Process ASSIMP's root node recursively
		this->processNode(scene->mRootNode, scene);
        loaded = 0;

	}

	void Model::processNode(aiNode* node, const aiScene* scene) {
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) {
        // data to fill
        std::vector<glm::vec4>* verticesConvexHull = nullptr;
        float* vtx = nullptr;
        uint32_t vertices_elem = 3; //3 pos
        if (mesh->HasNormals()) {
            vertices_elem += 3;
        }
        if (mesh->mTextureCoords[0]) {
            vertices_elem += 2;
            vertices_elem += 3; //tangent
            vertices_elem += 3; //bitangent
        }

        if (m_convexHull) {
            verticesConvexHull = new std::vector<glm::vec4>;
        }

        std::cout << vertices_elem << std::endl;
        uint32_t vertices_total = vertices_elem * mesh->mNumVertices;
        float* vertices = new float[vertices_total]; // +8 to avoid warnings
        std::vector<GLuint>* indices = new std::vector<GLuint>;
        std::vector<Texture*>* textures = new std::vector<Texture*>;

        // walk through each of the mesh's vertices
        int n_vertices = 0;
        for (unsigned int i = 0; i < vertices_total; i = i + vertices_elem){  
            // positions
            vertices[i] = mesh->mVertices[n_vertices].x;
            vertices[i + 1] = mesh->mVertices[n_vertices].y;
            vertices[i + 2] = mesh->mVertices[n_vertices].z;

            if (m_convexHull) {
                glm::vec4 vertex(vertices[i], vertices[i+1], vertices[i+2], 0.0f);
                if (!(std::count(verticesConvexHull->begin(), verticesConvexHull->end(), vertex))) {
                    verticesConvexHull->push_back(vertex);
                }
            }
 
            int j = 3;

            // normals
            if (mesh->HasNormals()){
                vertices[i + j] = mesh->mNormals[n_vertices].x;
                vertices[i + j + 1] = mesh->mNormals[n_vertices].y;
                vertices[i + j + 2] = mesh->mNormals[n_vertices].z;
                j = j + 3;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) {// does the mesh contain texture coordinates?
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vertices[i + j] = mesh->mTextureCoords[0][n_vertices].x;
                vertices[i + j + 1] = mesh->mTextureCoords[0][n_vertices].y;

                // tangent
                vertices[i + j + 2] = mesh->mTangents[0].x;
                vertices[i + j + 3] = mesh->mTangents[n_vertices].y;
                vertices[i + j + 4] = mesh->mTangents[n_vertices].z;
                // bitangent
                vertices[i + j + 5] = mesh->mBitangents[n_vertices].x;
                vertices[i + j + 6] = mesh->mBitangents[n_vertices].y;
                vertices[i + j + 7] = mesh->mBitangents[n_vertices].z;

            }
   
            n_vertices++;
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices->push_back(face.mIndices[j]);
        }

        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

       // 1. diffuse maps = albedo map (pbr)
        std::vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_albedo");
        textures->insert(textures->end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps = metallic map (pbr)
        std::vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_metallic");
        textures->insert(textures->end(), specularMaps.begin(), specularMaps.end());
        // 3. ambient maps = ao map (pbr)
        std::vector<Texture*> aoMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ao");
        textures->insert(textures->end(), aoMaps.begin(), aoMaps.end());
        // 4. normal maps
        std::vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures->insert(textures->end(), normalMaps.begin(), normalMaps.end());
        // 5. emissive maps = roughness map (pbr)
        std::vector<Texture*> roughnessMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_roughness");
        textures->insert(textures->end(), roughnessMaps.begin(), roughnessMaps.end());


        uint32_t size = vertices_total * sizeof(float);

        Mesh* newMesh = new Mesh(vertices, size, mesh->mNumVertices, indices, textures);
        if (m_convexHull) {
            newMesh->setConvexHull(verticesConvexHull);
        }
        return newMesh;
		
	}

	std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName){
        std::vector<Texture*> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++){
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            std::string path = this->directory + "/" + str.C_Str();
            for (unsigned int j = 0; j < textures_loaded.size(); j++){
                if (std::strcmp(textures_loaded[j]->getPath().c_str(), path.c_str()) == 0){
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip){   // if texture hasn't been loaded already, load it
                std::string path = this->directory + "/" + str.C_Str();
                Texture* texture = new Texture(path, typeName);
                //std::cout << path << std::endl;
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
	}




}