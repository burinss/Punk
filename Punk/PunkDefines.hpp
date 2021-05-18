#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <list>

class Task;

const glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);
const glm::vec3 WORLD_RIGHT(1.0f, 0.0f, 0.0f);
const glm::vec3 WORLD_FRONT(0.0f, 0.0f, -1.0f);

using PTaskList = std::list<std::shared_ptr<Task>>;

enum class PCameraProjectionMode
{
	Perspective, Orthographic
};

enum class PPlayState
{
	Playing, Paused, NotStarted
};

enum class PRenderOp
{
	Depth, Shadow, Color, Post, UI
};

struct PWindowSettings
{
    glm::vec2 windowSize;
    glm::vec2 resolution;
    bool maximized = true;
    float Aspect()
    {
        return windowSize.x / windowSize.y;
    }
};

struct PRenderSettings
{
    glm::vec4 outlineColor = glm::vec4(1.0f);
    glm::vec2 shadowMapResolution = glm::vec2(8192.f);
    bool enableMsaa = false;
};

struct PSystemVariables
{
    PWindowSettings windowSettings;
    PRenderSettings renderSettings;
};

enum  PTaskPriority
{
    FirstPriority, Critical = FirstPriority, High, Medium, Normal, Low, LastPriority
};

enum class TaskState
{
    Uninitialized, Removed, Running, Paused, Finished, Failed, Aborted
};

enum class PMeshDrawStyle
{
    Point, Line, LineStrip, Triangle, TriangleStrip, TriangleFan, Quads
};

enum class PTextureWrapping
{
    Repeat, EdgeClamp, BorderClamp
};

enum class PLightType
{
    Directional, Point, Spot, Area
};

struct PVertex2D
{
	glm::vec2 position;
	glm::vec2 uv;

    PVertex2D(const glm::vec2& position = glm::vec2(0.f), const glm::vec2& uv = glm::vec2(0.f)) : position(position), uv(uv) {}
};

struct PVertex3D {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 uv;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;

    PVertex3D()
    {
        
    }

    PVertex3D(glm::vec3 position, glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f)) : PVertex3D()
    {
        this->position = position;
        this->normal = normal;
    }
};

struct PVertex2DDataStruct
{
	std::vector<PVertex2D> vertices;
    unsigned int numVertices;
};

struct PVertex3DDataStruct
{
	std::vector<PVertex3D> vertices;
	std::vector<unsigned int> indices;
};