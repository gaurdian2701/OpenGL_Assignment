#include "ObjectDrawData.h"
#include "glad/glad.h"
#include "VertexAttributePointerData.h"

ObjectDrawData::ObjectDrawData(std::vector<float>& vertexAttributeData, std::vector<VertexAttributePointerData*>& vertexAttributePointerData,
	std::vector<unsigned int>& indices)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexAttributeData.size() * sizeof(float), vertexAttributeData.data(), GL_STATIC_DRAW);

	InitializeIndices(indices);
	InitializeVertexAttributePointer(vertexAttributePointerData);
}

ObjectDrawData::ObjectDrawData(unsigned int VBO,
	std::vector<VertexAttributePointerData*>& vertexAttributePointerData,
	std::vector<unsigned int> indices)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	m_VBO = VBO;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	InitializeIndices(indices);
	InitializeVertexAttributePointer(vertexAttributePointerData);
}

unsigned int ObjectDrawData::GetVAO() const
{
	return m_VAO;
}

unsigned int ObjectDrawData::GetVBO() const
{
	return m_VBO;
}

unsigned int ObjectDrawData::GetEBO() const
{
	return m_EBO;
}

void ObjectDrawData::InitializeIndices(std::vector<unsigned int>& indices)
{
	if (indices.size() > 0)
	{
		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}
}

void ObjectDrawData::InitializeVertexAttributePointer(std::vector<VertexAttributePointerData*>& vertexAttributePointerData)
{
	for (unsigned int i = 0; i < vertexAttributePointerData.size(); i++)
	{
		glEnableVertexAttribArray(vertexAttributePointerData[i]->m_index);
		glVertexAttribPointer(vertexAttributePointerData[i]->m_index,
			vertexAttributePointerData[i]->m_size,
			vertexAttributePointerData[i]->m_type,
			vertexAttributePointerData[i]->m_normalized,
			vertexAttributePointerData[i]->m_stride,
			(void*)vertexAttributePointerData[i]->m_startingOffset);
	}
}

