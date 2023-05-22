#pragma once

#include "Renderer/VertexBuffer.h"

namespace Spark
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* data, uint32_t size, VertexBufferUsage usage);
		OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage);
		~OpenGLVertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, uint32_t size, uint32_t offset) override;

		void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		const BufferLayout& GetLayout() const  override { return m_Layout; }

		uint32_t GetSize() const override { return m_Size; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Size = 0;
		BufferLayout m_Layout;
		VertexBufferUsage m_Usage;
	};
}