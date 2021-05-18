#pragma once

#include "PGameObject.hpp"
#include "PunkDefines.hpp"

class PMesh3D;

class PStaticMeshObject : public PGameObject
{
public:
	PStaticMeshObject(const glm::vec3& position = glm::vec3(0.f, 1.f, 0.f), const glm::quat& orientation = glm::quat(glm::vec3(0.f)))
		:PGameObject(position, orientation) {};
	PStaticMeshObject(const std::string& name) : PGameObject(name) {};

private:
	std::shared_ptr<PMesh3D> mesh_;
};