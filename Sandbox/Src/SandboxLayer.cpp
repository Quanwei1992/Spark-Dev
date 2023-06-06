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
		RenderCommand::SetClearColor(0.0f, 0.0f, 0.0f);
		constexpr  float quadVertices[] = {
			 -1, -1, 0,
			 1, -1, 0,
			 1,  1, 0,
			-1,  1, 0
		};

		int size = sizeof(quadVertices);

		const auto vertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		vertexBuffer->SetLayout({
			{ShaderDataType::Float3,"a_Position"}
		});

		constexpr uint32_t indices[] = { 0,1,2,2,3,0 };
		const auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices));

		m_QuadVertexArray = VertexArray::Create();
		m_QuadVertexArray->AddVertexBuffer(vertexBuffer);
		m_QuadVertexArray->SetIndexBuffer(indexBuffer);

		m_GridShader = Shader::Create("Assets/Shaders/Grid.glsl");

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

		// Draw Grid
		m_GridShader->Bind();
		const auto& viewProj = m_EditorCamera.GetProjectionMatrix() * m_EditorCamera.GetViewMatrix();
		const auto& inversedViewProj = glm::inverse(viewProj);

		m_GridShader->SetMat4("u_InversedViewProjection", inversedViewProj);
		m_GridShader->SetMat4("u_ViewProjection", viewProj);
		m_GridShader->SetFloat("u_LineWidth", 2.0f);
		m_GridShader->SetFloat("u_CameraDistance", abs(m_EditorCamera.GetPosition().y));
		m_QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(6);

	}


	void SandBoxLayer::OnDetach()
	{
		
	}

	void SandBoxLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);
	}
}
