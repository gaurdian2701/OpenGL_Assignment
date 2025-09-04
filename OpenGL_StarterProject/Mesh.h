#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>


class ShaderProgram;

struct VertexData {
	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	Mesh(VertexData vertexData, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(ShaderProgram* shaderProgram);

	VertexData m_vertexData;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

private:
	unsigned int VAO, VBO, EBO;
	void SetupMesh();
};

