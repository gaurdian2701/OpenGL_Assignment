#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class ShaderProgram;

class Model
{
public:
	Model(const char* modelPath);
	void Draw(ShaderProgram* shaderProgram);

private:
	std::vector<class Mesh> m_meshes;
	std::vector<Texture> m_loadedTextures;
	std::string m_meshDirectory = "";

	void LoadModel(std::string modelPath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType textureType, std::string textureTypename);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
};

