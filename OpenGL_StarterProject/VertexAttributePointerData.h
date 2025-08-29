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

	bool m_normalized = false;
	int m_index = 0;
	int m_size = 0;
	int m_type = 0;
	int m_stride = 0;
	unsigned int m_startingOffset = 0;
};