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

		virtual ~Shader() = default;

		static Ref<Shader> Create(const std::filesystem::path& filePath);
	};
}
