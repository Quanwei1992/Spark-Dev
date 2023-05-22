#pragma once

#include "Renderer/VertexArray.h"

namespace Spark
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer)  override;
		void SetIndexBuffer(Ref<IndexBuffer> indexBuffer)  override;
		const std::vector<Ref<VertexBuffer>> GetVertexBuffers() const override;
		const Ref<IndexBuffer> GetIndexBuffer() const override;

	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID = 0;
	};
}