#pragma once
#include "Core/Base.h"

namespace Spark
{
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;
		virtual uint32_t GetSize() const = 0;
		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t size);
	};
}