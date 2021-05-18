#pragma once
#include "PFrustum.hpp"
#include "PGameObject.hpp"
#include "PunkDefines.hpp"



class PLight :public PGameObject
{
public:
	bool enabled;

	PLightType type;
	glm::vec3 ambient{ 0.1f };
	glm::vec3 color{ 0.9f };
	float compression = 5.f;
	float radius = 100.f;

	PLight(const glm::vec3& position = glm::vec3(0.f, 1.f, 0.f), const glm::quat& orientation = glm::quat(glm::vec3(0.f)),PLightType lighttype=PLightType::Point)
	: PGameObject(position, orientation),enabled(true),type(lighttype) {};
	//PLight(const std::string& name="PunkLight") :PGameObject(name) {}
	~PLight() {};

	void Initialize() override { PGameObject::Initialize(); }

	glm::mat4 LightSpaceMatrix() const { return lightspaceMatrix_; }

	void UpdateLightspaceMatrix(const PFrustum& frustum);

private:

	glm::mat4 lightspaceMatrix_;
};