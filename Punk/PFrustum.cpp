#include "PFrustum.hpp"

PFrustum::PFrustum(float ffov, float fratio, float fnear, float ffar)
{
    this->m_fov = ffov;
    this->m_ratio = fratio;
    this->m_near = fnear;
    this->m_far = ffar;

    m_tang = (float)glm::tan(glm::radians(m_fov) * 0.5f);
    nearHeight = m_near * m_tang;
    nearWidth = nearHeight * m_ratio;
    farHeight = m_far * m_tang;
    farWidth = farHeight * m_ratio;

    Recalculate(glm::vec3(0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
}

void PFrustum::Recalculate(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up)
{
    glm::vec3 nearCenter, farCenter, x, y, z;

    z = glm::normalize(pos - front);
    x = glm::normalize(glm::cross(up, z));
    y = glm::cross(z, x);

    nearCenter = pos - z * m_near;
    farCenter = pos - z * m_far;

    m_corners[0] = nearCenter + y * nearHeight - x * nearWidth; // Near Top Left
    m_corners[1] = nearCenter + y * nearHeight + x * nearWidth; // Near Top Right
    m_corners[2] = nearCenter - y * nearHeight + x * nearWidth; // Near Bottom Right
    m_corners[3] = nearCenter - y * nearHeight - x * nearWidth; // Near Bottom Left
    m_corners[4] = farCenter + y * farHeight - x * farWidth;	  // Far Top Left
    m_corners[5] = farCenter + y * farHeight + x * farWidth;	  // Far Top Right
    m_corners[6] = farCenter - y * farHeight + x * farWidth;	  // Far Bottom Right
    m_corners[7] = farCenter - y * farHeight - x * farWidth;	  // Far Bottom Left

    m_planes[NEAR] = PPlane(m_corners[0], m_corners[0], m_corners[2]);
    m_planes[FAR] = PPlane(m_corners[5], m_corners[4], m_corners[7]);
    m_planes[TOP] = PPlane(m_corners[1], m_corners[0], m_corners[4]);
    m_planes[BOTTOM] = PPlane(m_corners[3], m_corners[2], m_corners[6]);
    m_planes[LEFT] = PPlane(m_corners[0], m_corners[3], m_corners[7]);
    m_planes[RIGHT] = PPlane(m_corners[2], m_corners[1], m_corners[6]);
}

bool PFrustum::Contains(const glm::vec3& point)
{
    for (int plane = 0; plane < NUMPLANES; plane++)
    {
        if (m_planes[plane].Distance(point) < 0.f)
            return false;
    }
    return true;
}

bool PFrustum::Contains(const glm::vec3& center, float radius)
{
    float distance;
    for (int plane = 0; plane < NUMPLANES; plane++)
    {
        distance = m_planes[plane].Distance(center);
        if (distance < -radius)
        {
            return false;
        }
    }
    return true;
}

bool PFrustum::Contains(const PFrustum& frustum)
{
    return true;
}
