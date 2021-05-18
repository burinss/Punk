#include "PLight.hpp"
#include <glm/gtc/matrix_transform.hpp>

void PLight::UpdateLightspaceMatrix(const PFrustum& frustum)
{
    glm::mat4 lightP = glm::perspective(glm::radians(179.0f), 1.f, 1.f, 1000.f);
    //glm::mat4 lightP = glm::ortho(-frustum.nearWidth, frustum.nearWidth, -frustum.nearHeight, -frustum.nearHeight, frustum.m_near, frustum.m_far);
    //glm::mat4 lightP = glm::ortho(-50.0f,50.0f,-50.0f,50.f, 0.01f, 1000.f);
    glm::mat4 lightV = glm::lookAt(/*type == PLightType::Directional ? glm::eulerAngles(Orientation()) : */Position(),glm::vec3(0.f), WORLD_UP);
   // glm::mat4 lightV = glm::lookAt(glm::eulerAngles(Orientation()), glm::vec3(0.f), WORLD_UP);
    lightspaceMatrix_ = lightP * lightV;
}
