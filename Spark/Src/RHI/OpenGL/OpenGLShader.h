#pragma once
#include "Renderer/Shader.h"

namespace Spark
{
	class OpenGLShader: public Shader
	{
	public:
		OpenGLShader(const std::filesystem::path& filename);
		const std::string& GetName() const override;
		void Bind() const override;
		void Unbind() const override;
		void SetInt(const std::string& name, int value) override;
		void SetFloat(const std::string& name, float value) override;
		void SetVector2(const std::string& name, const Vector2& value) override;
		void SetVector3(const std::string& name, const Vector3& value) override;
		void SetVector4(const std::string& name, const Vector4& value) override;
		void SetMat3(const std::string& name, const Mat3& matrix) override;
		void SetMat4(const std::string& name, const Mat4& matrix) override;
		~OpenGLShader() override;

	private:
		int GetUniformLocation(const std::string& name);

	private:
		uint32_t m_RendererID = 0;
		std::string m_Name;
		std::unordered_map<std::string, int> m_UniformsLocationCache;
	};
}