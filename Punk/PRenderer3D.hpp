#pragma once
#include "PunkDefines.hpp"

class PRenderPass;

class PRenderer3D
{

private:

	std::vector<std::shared_ptr<PRenderPass>> passes_;

public:

	PRenderer3D() = default;
	~PRenderer3D() = default;

	const std::vector<std::shared_ptr<PRenderPass>>& Passes() const { return passes_; }

	void Render(double deltaTime);

	void AddPass(const std::shared_ptr<PRenderPass>& pass);

};