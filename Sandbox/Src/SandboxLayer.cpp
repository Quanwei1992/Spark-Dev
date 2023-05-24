#include "SandboxLayer.h"

namespace Spark
{
	SandBoxLayer::SandBoxLayer()
		:Layer("SandBoxLayer")
	{
		
	}

	SandBoxLayer::~SandBoxLayer()
	{
	}

	void SandBoxLayer::OnUpdate(Timestep ts)
	{
		RenderCommand::Clear();

		const int indexCount = m_VertexArray->GetIndexBuffer()->GetCount();
		m_UnlitColorShader->Bind();
		m_VertexArray->Bind();
		RenderCommand::DrawIndexed(indexCount);
	}

	void SandBoxLayer::OnAttach()
	{
		RenderCommand::SetClearColor(0.2f, 0.2f, 0.2f);
		constexpr float vertices[] = {
			-0.5f,-0.5f,0.0f,
			0.5f,-0.5f,0.0f,
			0.0f,0.5f,0.0f
		};
		const auto vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->SetLayout({
			{ShaderDataType::Float3,"a_Position"}
		});

		constexpr uint32_t indices[] = { 0,1,2 };
		const auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices));

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_UnlitColorShader = Shader::Create("Assets/Shaders/UnlitColor.glsl");
		m_UnlitColorShader->SetVector4("u_Color", { 0.2f,0.8f,0.3f,1 });

		auto mesh = CreateRef<Mesh>("Assets/Models/Armature.fbx");

	}

	void SandBoxLayer::OnDetach()
	{
		
	}
}
