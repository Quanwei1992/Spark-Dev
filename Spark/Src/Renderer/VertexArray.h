#pragma once

#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

namespace Spark
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>> GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer> GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};

}
