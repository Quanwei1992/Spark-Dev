#pragma once

#include "Renderer/IndexBuffer.h"

namespace Spark
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* indices, uint32_t size);
		~OpenGLIndexBuffer() override;

		void SetData(const void* data, uint32_t size, uint32_t offset) override;
		void Bind() const override;
		void Unbind() const override;
		uint32_t GetCount() const override;
		uint32_t GetSize() const override { return m_Size; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Size = 0;
	};
}