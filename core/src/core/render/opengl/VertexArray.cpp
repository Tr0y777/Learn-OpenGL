#include "VertexArray.h"

#include "core/Logs.h"

#include "glad/glad.h"

namespace Bunny {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray() {
		glDeleteBuffers(1, &m_id);
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertex_array) noexcept {
		m_id = vertex_array.m_id;
		m_elem_count = vertex_array.m_elem_count;
		
		vertex_array.m_id = 0;
		vertex_array.m_elem_count = 0;

		return *this;
	}

	VertexArray::VertexArray(VertexArray&& vertex_array) noexcept
		: m_id(vertex_array.m_id)
		, m_elem_count(vertex_array.m_elem_count)
	{
		vertex_array.m_id = 0;
		vertex_array.m_elem_count = 0;
	}

	void VertexArray::bind() const {
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() {
		glBindVertexArray(0);
	}

	void VertexArray::add_buffer(const VertexBuffer& vertex_buffer) {
		bind();
		vertex_buffer.bind();

		glEnableVertexAttribArray(m_elem_count);
		glVertexAttribPointer(m_elem_count, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		++m_elem_count;
	}

}