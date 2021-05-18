#pragma once

#include "PunkDefines.hpp"

class PPlane
{

public:

    glm::vec3 center;
    glm::vec3 normal;

    PPlane() {}
    PPlane(const glm::vec3& c, const glm::vec3& n) : center(c), normal(n) {}
    PPlane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

    float Distance(const glm::vec3& point);
    glm::vec3 Intersection(const PPlane& a, const PPlane& b);

};