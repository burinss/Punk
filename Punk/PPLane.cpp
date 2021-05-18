#include "PPLane.hpp"

PPlane::PPlane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
    glm::vec3 e2 = p2 - p1, e1 = p3 - p1;
    normal = glm::normalize(glm::cross(e2, e1));
    center = normal * glm::dot(p1, normal);
}

float PPlane::Distance(const glm::vec3& point)
{
    return glm::length(glm::dot(point, glm::normalize(normal)) - center);
}

glm::vec3 PPlane::Intersection(const PPlane& a, const PPlane& b)
{
    float f = -glm::dot(normal, glm::cross(a.normal, b.normal));

    glm::vec3 v1(center * glm::cross(a.normal, b.normal));
    glm::vec3 v2(a.center * glm::cross(b.normal, normal));
    glm::vec3 v3(b.center * glm::cross(normal, a.normal));

    return (v1 + v2 + v3) / f;
}
