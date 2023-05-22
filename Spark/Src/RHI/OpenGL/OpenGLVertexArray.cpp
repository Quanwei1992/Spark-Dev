#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Spark
{
	namespace Utils
	{
		static GLenum ShaderDateTypeToOpenGLBaseType(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float: return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Mat3: return GL_FLOAT;
			case ShaderDataType::Mat4: return GL_FLOAT;
			case ShaderDataType::Int: return GL_INT;
			case ShaderDataType::Int2: return GL_INT;
			case ShaderDataType::Int3: return GL_INT;
			case ShaderDataType::Int4: return GL_INT;
			case ShaderDataType::Bool: return GL_BOOL;
			}
			SP_CORE_ASSERT(false, "Unkown ShaderDataType!");
			return 0;
		}
	}
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		const auto elements = vertexBuffer->GetLayout().GetElements();
		SP_CORE_ASSERT(elements.size() > 0, "Vertex Buffer has no layout!");

		Bind();
		vertexBuffer->Bind();

		uint32_t index = 0;
		for (const auto& element : vertexBuffer->GetLayout())
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					element.GetComponentCount(),
					Utils::ShaderDateTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					vertexBuffer->GetLayout().GetStride(),
					static_cast<const uint8_t*>(nullptr) + element.Offset);
				index++;
			}
			break;

			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index,
					element.GetComponentCount(),
					Utils::ShaderDateTypeToOpenGLBaseType(element.Type),
					vertexBuffer->GetLayout().GetStride(),
					static_cast<const uint8_t*>(nullptr) + element.Offset);
				index++;
			}
			break;

			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				const uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						count,
						Utils::ShaderDateTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						vertexBuffer->GetLayout().GetStride(),
						reinterpret_cast<const uint8_t*>(element.Offset + (sizeof(float)) * count * i));

					glVertexAttribDivisor(index, 1);
					index++;
				}
			}
			break;
			default:
				SP_CORE_ASSERT(false, "Unkown ShaderDataType!");
				break;
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
		Unbind();
		vertexBuffer->Unbind();
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		Bind();
		indexBuffer->Bind();
		Unbind();
		indexBuffer->Unbind();
		m_IndexBuffer = indexBuffer;
	}

	const std::vector<Ref<VertexBuffer>> OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const Ref<IndexBuffer> OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}