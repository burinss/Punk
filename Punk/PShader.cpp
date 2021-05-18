#include "PShader.hpp"
#include <glad/glad.h>

void PShader::CheckCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void PShader::Compile()
{
	const char* vShaderCode = _vertexCode.c_str();
	const char* fShaderCode = _pixelCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	CheckCompileErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	CheckCompileErrors(fragment, "FRAGMENT");

	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	if (_geometryShaderPath != "")
	{
		const char* gShaderCode = _geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);

		CheckCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	_id = glCreateProgram();
	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);

	if (_geometryShaderPath != "")
		glAttachShader(_id, geometry);
	glLinkProgram(_id);

	CheckCompileErrors(_id, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	if (_geometryShaderPath != "")
		glDeleteShader(geometry);
}

std::string PShader::GetShaderCode(const std::string& shaderPath)
{
	std::string shaderCode;
	std::ifstream ShaderFile;
	// ensure ifstream objects can throw exceptions:
	ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		ShaderFile.open(shaderPath);
		std::stringstream ShaderStream;
		// read file's buffer contents into streams
		ShaderStream << ShaderFile.rdbuf();
		// close file handlers
		ShaderFile.close();
		// convert stream into string
		shaderCode = ShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << shaderPath<< std::endl;
	}
	return shaderCode;
}

PShader::PShader()
{
}

PShader::PShader(const std::string& vertexShaderPath, const std::string& pixelShaderPath, const std::string& geometryShaderPath) :_vertexShaderPath(vertexShaderPath), _pixelShaderPath(pixelShaderPath), _geometryShaderPath(geometryShaderPath)
{
	
}

void PShader::Initialize()
{
	_vertexCode = GetShaderCode(_vertexShaderPath);

	_pixelCode = GetShaderCode(_pixelShaderPath);
	if(_geometryShaderPath!="")
	_geometryCode = GetShaderCode(_geometryShaderPath);

	Compile();
}

void PShader::Use()
{
	glUseProgram(_id);
}

void PShader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void PShader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void PShader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void PShader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}
void PShader::SetVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(_id, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void PShader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}
void PShader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void PShader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}
void PShader::SetVec4(const std::string& name, float x, float y, float z, float w)const
{
	glUniform4f(glGetUniformLocation(_id, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void PShader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void PShader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void PShader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
