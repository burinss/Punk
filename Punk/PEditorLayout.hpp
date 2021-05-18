#pragma once

#include "PunkDefines.hpp"
#include "PTask.hpp"

class PScene;
class PIDSequence;
class PEditorWindow;


class PEditorLayout: public Task, public std::enable_shared_from_this<PEditorLayout>
{
public:
	PEditorLayout(const std::shared_ptr<PScene>& sceneContext) :m_sceneContext(sceneContext) {}
	~PEditorLayout() {};

	void Initialize() override;
	void Update(double deltaTime) override;
	void CleanUp() override;

	auto GetContext() { return m_sceneContext.lock(); }
protected:
	std::vector<std::shared_ptr<PEditorWindow>> m_windows;
	std::weak_ptr<PScene> m_sceneContext;
	//static PIDSequence m_idGen;
private:
};