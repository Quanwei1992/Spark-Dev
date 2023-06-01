#pragma once
#include <Spark.h>

namespace Spark
{
	class SandBoxLayer : public Layer
	{
	public:
		SandBoxLayer();
		~SandBoxLayer() override;
		void OnUpdate(Timestep ts) override;
		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& event) override;

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_UnlitColorShader;
		Ref<Shader> m_LightingShader;
		EditorCamera m_EditorCamera;
			
		Mat4 m_ModelMatrix;
		Ref<Mesh> m_NanosuitMesh;
		Ref<MeshRenderer> m_NanosuitMeshRenderer;
	};
}
