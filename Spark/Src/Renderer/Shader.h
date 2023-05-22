#pragma once

#include "Core/Base.h"

#include <filesystem>

namespace Spark
{
	class Shader
	{
	public:
		virtual const std::string& GetName() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetVector2(const std::string& name, const Vector2& value) = 0;
		virtual void SetVector3(const std::string& name, const Vector3& value) = 0;
		virtual void SetVector4(const std::string& name, const Vector4& value) = 0;
		virtual void SetMat3(const std::string& name, const Mat3& matrix) = 0;
		virtual void SetMat4(const std::string& name, const Mat4& matrix) = 0;


		virtual ~Shader() = default;

		static Ref<Shader> Create(const std::filesystem::path& filePath);
	};
}
