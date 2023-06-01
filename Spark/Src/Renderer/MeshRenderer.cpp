#include "MeshRenderer.h"
#include "Renderer/RenderCommand.h"
namespace Spark
{
	MeshRenderer::MeshRenderer(const Ref<Mesh>& mesh)
		:m_Mesh(mesh)
	{
		const auto& vertices = mesh->GetVertices();
		const auto& indices = mesh->GetIndices();

		m_VertexBuffer = VertexBuffer::Create(vertices.data(), sizeof(Vertex) * vertices.size());
		m_IndexBuffer = IndexBuffer::Create(indices.data(), sizeof(Index) * indices.size());


		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3,"aPosition"},
			{ShaderDataType::Float3,"aNormal"},
			{ShaderDataType::Float3,"aTangent"},
			{ShaderDataType::Float3,"aBinormal"},
			{ShaderDataType::Float2,"aTexcoord"},
		});

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	}

	void MeshRenderer::Draw(const Mat4& transform, const Ref<Shader>& shader) const
	{
		
		m_VertexArray->Bind();

		const auto& submeshes = m_Mesh->GetSubmeshs();
		for(const auto& submesh : submeshes)
		{
			Mat4 model = transform * submesh.Transform;
			shader->SetMat4("u_Model", model);
			RenderCommand::DrawIndexed(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex);
		}

		m_VertexArray->Unbind();
	}
}
