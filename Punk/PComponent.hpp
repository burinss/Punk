#pragma once
#include "PunkDefines.hpp"
#include "PTask.hpp"

class PGameObject;


class PComponent: public Task
{
	friend class ZGameObject;
public:

    virtual ~PComponent() {}

    virtual void Initialize() { Task::Initialize(); }
    //virtual void Initialize(std::shared_ptr<ZOFNode> root) = 0;

    virtual void CleanUp() { Abort(); }

    //virtual std::shared_ptr<PComponent> Clone() = 0;

    PGameObject* Object() { return m_object; }

    static std::shared_ptr<PComponent> CreateMeshComponent(const std::shared_ptr<PGameObject>& gameObject);
    //static std::shared_ptr<PComponent> CreatePhysicsComponent(const std::shared_ptr<PGameObject>& gameObject);
    //static std::shared_ptr<PComponent> CreateAnimatorComponent(const std::shared_ptr<PGameObject>& gameObject);

//protected:

    PGameObject* m_object;

};