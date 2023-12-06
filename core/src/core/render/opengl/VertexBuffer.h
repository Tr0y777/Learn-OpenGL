#pragma once

#include <vector>

namespace Bunny {

	enum class ShaderDataType {
		FLoat,
		FLoat2,
		FLoat3,
		FLoat4,
		Int,
		Int2,
		Int3,
		Int4,
	};

	struct BufferElement {
		ShaderDataType type;
		uint32_t component_type;
		size_t components_count;
		size_t size;
		size_t offset;

		BufferElement(const ShaderDataType type);
	};

	class BufferLayout {
	public:
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_elem(std::move(elements))
		{
			size_t offset = 0;
			m_stride = 0;
			for (auto& elem : m_elem) {
				elem.offset = offset;
				offset += elem.size;
				m_stride += elem.size;
			}
		}

		const std::vector<BufferElement>& get_elem() const { return m_elem; }
		size_t get_stride() const { return m_stride; }

	private:
		std::vector<BufferElement> m_elem;
		size_t m_stride = 0;
	};

	class VertexBuffer {
	public:
		enum class EUsage
		{
			Static,
			Dynamic,
			Stream
		};

		VertexBuffer(const void* data, const size_t size, BufferLayout buffer_layout, const EUsage usage = VertexBuffer::EUsage::Static);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&& vertex_buffer) noexcept;
		VertexBuffer(VertexBuffer&& vertex_buffer) noexcept;

		void bind() const;
		static void unbind();

		const BufferLayout& get_layout() const { return m_buffer_layout; }
	private:
		unsigned int m_id = 0;
		BufferLayout m_buffer_layout;
	};

}