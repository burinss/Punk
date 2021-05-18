#pragma once
#include "PunkDefines.hpp"
#include "PRenderable.hpp"
#include "PTask.hpp"

class PComponent;
class PScene;
class PIDSequence;
//GameObject Properties
struct PGOProp
{
//ZRenderOrder renderOrder;
glm::vec4 position, previousPosition;
glm::vec3 scale, previousScale;
glm::quat orientation, previousOrientation;
glm::mat4 modelMatrix;
std::string name;
};

class PGameObject: public Task, public PRenderable , public std::enable_shared_from_this<PGameObject>
{
    friend class PScene;
public:
    PGameObject(const glm::vec3& position = glm::vec3(0.f, 1.f, 0.f), const glm::quat& orientation = glm::quat(glm::vec3(0.f)));
    PGameObject(const std::string& name) : PGameObject() { m_properties.name = name; }
    virtual ~PGameObject() {}

    virtual void Initialize() override { Task::Initialize(); }
   //virtual void Initialize(std::shared_ptr<ZOFNode> root);

    virtual void PreRender();
    virtual void Render(double deltaTime, const std::shared_ptr<PShader>& shader/*, ZRenderOp renderOp = ZRenderOp::Color*/) override;
    virtual void RenderChildren(double deltaTime, const std::shared_ptr<PShader>& shader/*, ZRenderOp renderOp = ZRenderOp::Color*/);
    virtual void PostRender();
    virtual bool Renderable() override { return true; }

    void CalculateDerivedData();
    //virtual std::shared_ptr<PGameObject> Clone();
    virtual void AddChild(std::shared_ptr<PGameObject> gameObject);
    virtual void RemoveChild(std::shared_ptr<PGameObject> gameObject, bool recurse = false);
    bool HasChildren() { return !m_childrens.empty(); }
    virtual bool IsVisible() { return true; }
    virtual void Destroy();

    std::shared_ptr<PScene> Scene() const;
    std::shared_ptr<PGameObject> Parent() const { return m_parent.lock(); };
    std::string Name() const { return m_properties.name; }
    //ZRenderOrder RenderOrder() const { return m_properties.renderOrder; }
    virtual std::vector<std::shared_ptr<PGameObject>>& Children() { return m_childrens; }
    glm::vec3 Position();
    glm::vec3 Scale();
    glm::quat Orientation();
    glm::vec3 Front();
    glm::vec3 Up();
    glm::vec3 Right();
    glm::mat4 ModelMatrix();
    glm::vec3 PreviousPosition();
    glm::vec3 PreviousFront();
    glm::vec3 PreviousUp();
    glm::vec3 PreviousRight();

    void SetPosition(const glm::vec3& position);
    void SetScale(const glm::vec3& scale);
    void SetOrientation(const glm::quat& quaternion);
    void SetOrientation(const glm::vec3& euler);
    void SetModelMatrix(const glm::mat4& modelMatrix);
    //void SetRenderOrder(ZRenderOrder renderOrder) { m_properties.renderOrder = renderOrder; }
    void SetName(const std::string& name) { m_properties.name = name; }


    template<class T>
    typename std::enable_if<std::is_base_of<PComponent, T>::value>::type
        AddComponent(std::shared_ptr<T> component)
    {
        std::shared_ptr<T> foundComponent = FindComponent<T>();
        if (foundComponent == nullptr)
        {
            component->m_object = this;
            m_components.push_back(component);
        }
    }

    template<class T>
    std::shared_ptr<T> RemoveComponent()
    {
        std::vector<std::shared_ptr<PComponent>>::iterator found;
        for (std::vector<std::shared_ptr<PComponent>>::iterator it = m_components.begin(); it != m_components.end(); ++it)
        {
            if (std::dynamic_pointer_cast<T>(*it))
            {
                found = it; break;
            }
        }

        if (found == m_components.end()) return nullptr;

        std::shared_ptr<T> removed = *found;
        m_components.erase(found);
        return removed;
    }

    template<class T>
    std::shared_ptr<T> FindComponent()
    {
        for (std::shared_ptr<PComponent> comp : m_components)
        {
            if (std::dynamic_pointer_cast<T>(comp)) return std::dynamic_pointer_cast<T>(comp);
        }
        return nullptr;
    }

	template<class T>
	std::shared_ptr<T> Child(const std::string& id)
	{
		if (!std::is_base_of<PGameObject, T>::value) return nullptr;

		std::shared_ptr<T> go;
		for (auto it = m_childrens.begin(); it != m_childrens.end(); it++)
		{
			if ((go = std::dynamic_pointer_cast<T>(*it)) && (go->ID() == id))
			{
				return go;
			}
			go = nullptr;
		}

		return go;
	}

protected:

    std::vector<std::shared_ptr<PGameObject>> m_childrens;
    std::vector<std::shared_ptr<PComponent>> m_components;
    std::weak_ptr<PGameObject> m_parent;
    PGOProp m_properties;

    std::weak_ptr<PScene> m_sceneContext;

    static PIDSequence m_idGenerator;
};