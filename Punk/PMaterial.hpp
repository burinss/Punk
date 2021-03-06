#pragma once

#include "PIDSequence.hpp"
#include "PTask.hpp"

class PShader;
class PTexture;

class PMaterial: public Task
{
public:
	PMaterial() { ::Task(); }

	void Initialize() override;

	void Use(std::shared_ptr<PShader> shader);
	void Set(std::string slot, std::shared_ptr<PTexture> texture);
	std::shared_ptr<PTexture> Get(std::string slot);

	std::string& GetName() { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	std::map<std::string, std::shared_ptr<PTexture>> GetSlots() { return m_textureSlots; }
private:
	std::string m_name;
	std::map<std::string, std::shared_ptr<PTexture>> m_textureSlots;
	static PIDSequence m_idGen;
};