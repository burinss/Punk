#pragma once
#include "PunkDefines.hpp"

class PShader {
private:
	unsigned int _id=0;

	std::string _vertexCode;
	std::string _pixelCode;
	std::string _geometryCode;

	std::string _vertexShaderPath;
	std::string _pixelShaderPath;
	std::string _geometryShaderPath;

	void CheckCompileErrors(unsigned int shader, std::string type);
	void Compile();
	std::string GetShaderCode(const std::string& shaderPath);
public:
	PShader();
	PShader(const std::string& vertexShaderPath, const std::string& pixelShaderPath, const std::string& geometryShaderPath);
	void Initialize();
	void Use();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, float x, float y) const;
	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetVec4(const std::string& name, float x, float y, float z, float w)const;
	void SetMat2(const std::string& name, const glm::mat2& mat) const;
	void SetMat3(const std::string& name, const glm::mat3& mat) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;

	unsigned int GetID() const { return _id; }
};