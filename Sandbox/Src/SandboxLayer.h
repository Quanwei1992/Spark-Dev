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
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_UnlitColorShader;
	};
}
