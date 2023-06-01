#include "SandboxLayer.h"

#include "Core/Window.h"

namespace Spark
{
	SandBoxLayer::SandBoxLayer()
		:Layer("SandBoxLayer")
	{
		
	}

	SandBoxLayer::~SandBoxLayer()
	{
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

		m_LightingShader = Shader::Create("Assets/Shaders/PhoneLighting_Color.glsl");
		m_LightingShader->SetVector3("u_ObjectColor", { 1.0f, 1.0f, 1.0f });
		m_LightingShader->SetVector3("u_LightColor", { 1.0f, 1.0f, 1.0f });
		m_LightingShader->SetVector3("u_LightPos", { 1.2f, 1.0f, 2.0f });

		//auto mesh = CreateRef<Mesh>("Assets/Models/Armature.fbx");

		const auto windowWidth = Application::Get().GetWindow().GetWidth();
		const auto windowHeight = Application::Get().GetWindow().GetHeight();
		m_EditorCamera.SetPosition({ 0,0,10 });
		m_EditorCamera.SetProjectionMatrixMode(ProjectionMatrixMode::Perspective);
		m_EditorCamera.SetAspect(static_cast<float>(windowWidth) / windowHeight);
		m_EditorCamera.SetViewportSize(windowWidth, windowHeight);

		m_ModelMatrix = Mat4(1.0f);

		m_NanosuitMesh = CreateRef<Mesh>("Assets/Models/Nanosuit.obj");
		//m_NanosuitMesh = Mesh::CreateQubeMesh();

		m_NanosuitMeshRenderer = CreateRef<MeshRenderer>(m_NanosuitMesh);
	}

	void SandBoxLayer::OnUpdate(Timestep ts)
	{

		m_EditorCamera.Update(ts);

		RenderCommand::Clear();
		m_LightingShader->Bind();
		m_LightingShader->SetMat4("u_View", m_EditorCamera.GetViewMatrix());
		m_LightingShader->SetMat4("u_Projection", m_EditorCamera.GetProjectionMatrix());

		m_NanosuitMeshRenderer->Draw(m_ModelMatrix, m_LightingShader);
	}


	void SandBoxLayer::OnDetach()
	{
		
	}

	void SandBoxLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);
	}
}
