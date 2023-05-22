#include "OpenGLShader.h"
#include <glad/glad.h>

namespace Spark
{
	namespace Utils
	{
		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex") return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
			SP_CORE_ASSERT(false, "Unkonwn shader type!");
			return 0;
		}

		std::string ReadFile(const std::filesystem::path& filePath)
		{
			std::string result;
			std::ifstream is(filePath, std::ios::in, std::ios::binary);
			if (is)
			{
				is.seekg(0, is.end);
				result.resize(is.tellg());
				is.seekg(0, is.beg);
				is.read(&result[0], result.size());
				is.close();
			}
			else {
				SP_CORE_ERROR("Count not open file '{0}'", filePath);
			}
			return result;
		}

		std::unordered_map<GLenum, std::string> PreProcessShader(const std::string& source)
		{
			std::unordered_map<GLenum, std::string> shaderSources;
			const char* typeToken = "#type";
			const size_t typeTokenLength = strlen(typeToken);
			size_t pos = source.find(typeToken, 0);
			while (pos != std::string::npos)
			{
				const size_t eol = source.find_first_of("\r\n", pos);
				SP_CORE_ASSERT(eol != std::string::npos, "Syntax error");
				const size_t begin = pos + typeTokenLength + 1;
				std::string type = source.substr(begin, eol - begin);
				SP_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified!");

				const size_t nextLinePos = source.find_first_of("\r\n", eol);
				pos = source.find(typeToken, nextLinePos);
				shaderSources[ShaderTypeFromString(type)]
					= source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
			}
			return shaderSources;
		}


		int CompileShader(const std::unordered_map<GLenum, std::string>& shaderSouces)
		{
			std::vector<GLuint> compiledShaders;

			for (auto& kv : shaderSouces)
			{
				GLenum type = kv.first;
				const std::string& source = kv.second;

				GLuint shader = glCreateShader(type);
				const GLchar* sourceCStr = source.c_str();
				glShaderSource(shader, 1, &sourceCStr, 0);
				glCompileShader(shader);
				GLint isCompiled = 0;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
				if (isCompiled == GL_FALSE)
				{
					GLint maxLength = 0;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

					std::vector<GLchar> infoLog(maxLength);
					glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

					glDeleteShader(shader);

					for (GLuint compiledShader : compiledShaders)
					{
						glDeleteShader(compiledShader);
					}
					compiledShaders.clear();
					SP_CORE_ERROR("{0}", infoLog.data());
					SP_CORE_ASSERT(false, "Shader compilation failure!");
					return 0;
				}
				compiledShaders.emplace_back(shader);
			}

			const GLuint program = glCreateProgram();


			for (const GLuint compiledShader : compiledShaders)
			{
				glAttachShader(program, compiledShader);
			}

			// Link our program
			glLinkProgram(program);

			// Note the different functions here: glGetProgram* instead of glGetShader*.
			GLint isLinked = 0;
			glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
			if (isLinked == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

				// We don't need the program anymore.
				glDeleteProgram(program);
				// Don't leak shaders either.
				for (const GLuint compiledShader : compiledShaders)
				{
					glDeleteShader(compiledShader);
				}
				compiledShaders.clear();

				SP_CORE_ERROR("{0}", infoLog.data());
				SP_CORE_ASSERT(false, "Shader link failure!");
				return 0;
			}

			// Always detach shaders after a successful link.
			for (const GLuint compiledShader : compiledShaders)
			{
				glDeleteShader(compiledShader);
			}
			return program;
		}

	}


	OpenGLShader::OpenGLShader(const std::filesystem::path& filename)
	{
		m_Name = filename.stem().string();
		const auto& source = Utils::ReadFile(filename);
		const auto& sources = Utils::PreProcessShader(source);
		m_RendererID = Utils::CompileShader(sources);
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		const auto it = m_UniformsLocationCache.find(name);
		if (it != m_UniformsLocationCache.end()) return it->second;

		const int location = glGetUniformLocation(m_RendererID, name.c_str());
		SP_CORE_ASSERT(location != -1, "Not found uniform location!");
		return location;
	}


	const std::string& OpenGLShader::GetName() const
	{
		return m_Name;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		const int location = GetUniformLocation(name);
		glProgramUniform1i(m_RendererID,location, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		const int location = GetUniformLocation(name);
		glProgramUniform1f(m_RendererID,location, value);
	}

	void OpenGLShader::SetVector2(const std::string& name, const Vector2& value)
	{
		const int location = GetUniformLocation(name);
		glProgramUniform2fv(m_RendererID,location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetVector3(const std::string& name, const Vector3& value)
	{
		const int location = GetUniformLocation(name);
		glProgramUniform3fv(m_RendererID,location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetVector4(const std::string& name, const Vector4& value)
	{
		const int location = GetUniformLocation(name);
		glProgramUniform4fv(m_RendererID,location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat3(const std::string& name, const Mat3& matrix)
	{
		const int location = GetUniformLocation(name);
		glProgramUniformMatrix3fv(m_RendererID,location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMat4(const std::string& name, const Mat4& matrix)
	{
		const int location = GetUniformLocation(name);
		glProgramUniformMatrix4fv(m_RendererID,location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	OpenGLShader::~OpenGLShader()
	{
		if(m_RendererID != 0)
		{
			glDeleteShader(m_RendererID);
		}
	}


}
