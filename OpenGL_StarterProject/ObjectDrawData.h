#pragma once
#include <vector>

struct VertexAttributePointerData;

class ObjectDrawData
{
public:
	ObjectDrawData(std::vector<float>* vertexAttributeData,
		std::vector<VertexAttributePointerData*>* vertexAttributePointerData,
		std::vector<unsigned int>* indices);

	ObjectDrawData(unsigned int VBO, 
		std::vector<VertexAttributePointerData*>* vertexAttributePointerData,
		std::vector<unsigned int>* indices);

	unsigned int GetVAO() const;
	unsigned int GetVBO() const;
	unsigned int GetEBO() const;

private:
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;

	std::vector<float> m_vertexAttributeData = {};
	std::vector<VertexAttributePointerData> m_vertexAttributePointerData = {};
	std::vector<unsigned int> m_indices = {};

	void InitializeIndices(std::vector<unsigned int>& indices);
	void InitializeVertexAttributePointer(std::vector<VertexAttributePointerData>& vertexAttributePointerData);
};

