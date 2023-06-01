#pragma once
#include "Core/Base.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
namespace Spark
{
	class MeshRenderer
	{
	public:
		MeshRenderer(const Ref<Mesh>& mesh);

		void Draw(const Mat4& transform,const Ref<Shader>& shader) const;

	private:
		const Ref<Mesh>& m_Mesh;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<VertexArray> m_VertexArray;
	};
}

