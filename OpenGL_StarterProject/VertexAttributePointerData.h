#pragma once
struct VertexAttributePointerData
{
public: 
	VertexAttributePointerData(int index, int size, int type, bool normalized, int stride, int startingOffset)
	{
		m_index = index;
		m_size = size;
		m_type = type;
		m_normalized = normalized;
		m_stride = stride;
		m_startingOffset = startingOffset;
	}

	VertexAttributePointerData(const VertexAttributePointerData& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_type = other.m_type;
		m_normalized = other.m_normalized;
		m_stride = other.m_stride;
		m_startingOffset = other.m_startingOffset;
	}

	bool m_normalized = false;
	int m_index = 0;
	int m_size = 0;
	int m_type = 0;
	int m_stride = 0;
	unsigned int m_startingOffset = 0;
};