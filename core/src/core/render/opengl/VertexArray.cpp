#include "VertexArray.h"

#include "core/Logs.h"

#include <glad/glad.h>

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

	void VertexArray::add_vertex_buffer(const VertexBuffer& vertex_buffer) {
		bind();
		vertex_buffer.bind();

		for (const BufferElement& current_elem : vertex_buffer.get_layout().get_elem()) {
			glEnableVertexAttribArray(m_elem_count);
			glVertexAttribPointer(
				m_elem_count,
				static_cast<GLint>(current_elem.components_count),
				current_elem.component_type,
				GL_FALSE,
				static_cast<GLsizei>(vertex_buffer.get_layout().get_stride()),
				reinterpret_cast<const void*>(current_elem.offset)
			);

			++m_elem_count;
		}
	}

	void VertexArray::set_index_buffer(const IndexBuffer& index_buffer) {
		bind();
		index_buffer.bind();
		m_indices_count = index_buffer.get_count();
	}

}