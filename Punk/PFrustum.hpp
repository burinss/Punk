#pragma once

#include "PunkDefines.hpp"
#include "PPLane.hpp"

class PFrustum
{

public:

    enum { TOP, BOTTOM, LEFT, RIGHT, NEAR, FAR, NUMPLANES };

    PPlane m_planes[NUMPLANES];
    glm::vec3 m_corners[8];
    float m_near, m_far, m_ratio, m_fov, m_tang;
    float nearWidth, nearHeight, farWidth, farHeight;

    PFrustum() {}
    PFrustum(float ffov, float fratio, float fnear, float ffar);

    void Recalculate(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up);
    bool Contains(const glm::vec3& point);
    bool Contains(const glm::vec3& center, float radius);
    //bool Contains(const ZAABBox& box);
    bool Contains(const PFrustum& frustum);

};