#include "Mesh.h"
#include "ShaderProgram.h"
#include "glad/glad.h"
#include "Model.h"

Mesh::Mesh(VertexData vertexData, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	m_vertexData = vertexData;
	m_indices = indices;
	m_textures = textures;
	SetupMesh();
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    size_t positionsArraySize = sizeof(glm::vec3) * m_vertexData.Positions.size();
    size_t normalsArraySize = sizeof(glm::vec3) * m_vertexData.Normals.size();
    size_t texCoordsArraySize = sizeof(glm::vec2) * m_vertexData.TexCoords.size();

    glBufferData(GL_ARRAY_BUFFER,
        positionsArraySize + normalsArraySize + texCoordsArraySize,
        nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, positionsArraySize, m_vertexData.Positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, positionsArraySize, normalsArraySize, m_vertexData.Normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, positionsArraySize + normalsArraySize,
        texCoordsArraySize, m_vertexData.TexCoords.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
        m_indices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
        (void*)positionsArraySize);
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,
        (void*)(positionsArraySize + normalsArraySize));

    glBindVertexArray(0);
}

void Mesh::Draw(ShaderProgram* shaderProgram)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = m_textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);

        shaderProgram->SetInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
