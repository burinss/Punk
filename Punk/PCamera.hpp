#pragma once
#include <glm/glm.hpp>
#include "PGameObject.hpp"
#include "PFrustum.hpp"
class PShader;

class PCamera: public PGameObject  {
private:
	// Camera Attributes
	bool isPrimary_ = false;
	bool movementEnabled_ = true;
	bool lookEnabled_ = true;
	float movementSpeed_ = 25.f;
	float lookSensitivity_ = 0.4f;
	float zoom_ = 45.0f;
	float zoomSpeed_ = 5.0f;
	float nearClippingPlane_ = 0.01f;
	float farClippingPlane_ = 1000.0f;
	float cameraDamping_ = 0.02f;
	//ZCameraType cameraType_;
	//ZCameraMovementStyle movementStyle_;
	PFrustum frustum_;
	glm::vec3 pitchVelocity_, yawVelocity_;
	glm::quat pitch_, yaw_;

	double currentDeltaTime_ = 0.0;
	double lastDeltaTime_ = 0.0;
	float frameMix_ = 0.f;

public:
	PCamera(const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),const glm::quat& orientation = glm::quat(glm::vec3(0.f)));
	
	~PCamera() {};

	void Initialize() override {
		PGameObject::Initialize();
	};
	void CleanUp() override {
		PGameObject::CleanUp();
	};

	glm::mat4 ViewMatrix();
	
	glm::mat4 ProjectionMatrix();
	
	void Render(double deltaTime, const std::shared_ptr<PShader>& shader)override;
	
	float NearField() const { return nearClippingPlane_; }
	float FarField() const { return farClippingPlane_; }
	PFrustum& Frustum() { return frustum_; }
	float Zoom() const { return zoom_; }
	void SetZoom(const float zoom) { zoom_ = zoom; }
	float Speed() const { return movementSpeed_; }

	void Move(float x=0.0f, float y=0.0f, float z=0.0f, bool useWorldFront = false);

	void Look(float pitch, float yaw);

	void UpdateCamera();
};