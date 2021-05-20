#include "PSceneRoot.h"

void PSceneRoot::Render(double deltaTime, const std::shared_ptr<PShader>& shader, PRenderOp renderOp)
{
	RenderChildren(deltaTime, shader, renderOp);
}
