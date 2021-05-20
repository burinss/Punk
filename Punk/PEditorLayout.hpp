#pragma once

#include "PunkDefines.hpp"
#include "PTask.hpp"

class PScene;
class PIDSequence;
class PEditorWindow;
class PTexture;

class PEditorLayout: public Task, public std::enable_shared_from_this<PEditorLayout>
{
public:
	PEditorLayout(const std::shared_ptr<PScene>& sceneContext) :m_sceneContext(sceneContext) {}
	~PEditorLayout() {};

	void Initialize() override;

	void Update(double deltaTime) override;
	void CleanUp() override;

	auto GetContext() const{ return m_sceneContext.lock(); }
	auto GetIconMap() const { return m_iconMap; }
	std::shared_ptr<PTexture> GetIcon(std::string name)const { return m_iconMap.at(name); }
	void PushIcon(std::string name, std::shared_ptr<PTexture> icon) { m_iconMap.insert({ name,icon }); }

protected:

	void LoadIconImages();
	
	std::map <std::string, std::shared_ptr<PTexture>> m_iconMap;
	std::vector<std::shared_ptr<PEditorWindow>> m_windows;
	std::weak_ptr<PScene> m_sceneContext;
	//static PIDSequence m_idGen;
private:
};