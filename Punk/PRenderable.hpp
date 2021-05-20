#pragma once

#include "PunkDefines.hpp"

// Forward Declarations
class PShader;

// Class and Data Structure Definitions
class PRenderable
{

public:

    virtual void Render(double deltaTime, const std::shared_ptr<PShader>& shader,PRenderOp renderOP = PRenderOp::Color) = 0;
    virtual bool Renderable() = 0;

};
