#include "PunkDefines.hpp"
#include "PCamera.hpp"
#include "PShader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PScene.hpp"

PCamera::PCamera(const glm::vec3 position, const glm::quat& orientation)
	: PGameObject(position, orientation)
{
	zoom_ =  100.f;
	pitch_ = glm::quat(0.f, glm::vec3(1.f, 0.f, 0.f));
	yaw_ = glm::quat(0.f, glm::vec3(0.f, 1.f, 0.f));
	pitchVelocity_ = glm::vec3(0.f);
	yawVelocity_ = glm::vec3(0.f);
	frustum_ = PFrustum(zoom_, 1.7777f, nearClippingPlane_, farClippingPlane_);
}

glm::mat4 PCamera::ViewMatrix()
{
	return glm::lookAt(Position(), Position() + Front(), Up());
}

glm::mat4 PCamera::ProjectionMatrix()
{
	glm::mat4 projectionMatrix(1.f);
	float aspect = 1.7777f;
	if (auto scene = m_sceneContext.lock())
		if (scene->GameConfig().windowSettings.windowSize.x > 0)
			aspect = scene->GameConfig().windowSettings.Aspect();

	projectionMatrix = glm::perspective(
		glm::radians(zoom_),
		aspect,
		nearClippingPlane_,
		farClippingPlane_);

	return projectionMatrix;
}

void PCamera::Render(double deltaTime, const std::shared_ptr<PShader>& shader)
{

	lastDeltaTime_ = currentDeltaTime_;
	currentDeltaTime_ = deltaTime;
	frameMix_ = glm::clamp(
		glm::abs(currentDeltaTime_ - lastDeltaTime_),
		0.0,
		1.0
	);

	UpdateCamera();
	//shader->SetMat4("view");
}

void PCamera::Move(float x, float y, float z, bool useWorldFront)
{
	float velocity = movementSpeed_* (float)currentDeltaTime_;
	glm::vec3 newPos = Position() + ((useWorldFront ? WORLD_FRONT : Front()) * z * velocity) + (Right() * x * -velocity)+ (Up() * y * velocity);
	SetPosition(newPos);

	frustum_.Recalculate(Position(), Position() + Front(), Up());
}

void PCamera::Look(float pitch, float yaw)
{
	pitch_ = glm::angleAxis(glm::radians(-pitch * lookSensitivity_), glm::vec3(1.f, 0.f, 0.f));
	yaw_ = glm::angleAxis(glm::radians(yaw * lookSensitivity_), glm::vec3(0.f, 1.f, 0.f));
	SetOrientation(glm::normalize(pitch_ * Orientation() * yaw_));
}

void PCamera::UpdateCamera()
{
	frustum_.Recalculate(Position(), Position() + Front(), Up());
	//if (!lookEnabled_) return;
	/*glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));*/
	//pitchVelocity_ *= glm::pow(cameraDamping_, (float)currentDeltaTime_);
	//yawVelocity_ *= glm::pow(cameraDamping_, (float)currentDeltaTime_);
	//pitch_ = glm::quat(pitchVelocity_ * (float)currentDeltaTime_);
	//yaw_ = glm::quat(yawVelocity_ * (float)currentDeltaTime_);
	//SetOrientation(glm::normalize(pitch_ * Orientation() * yaw_));
}
