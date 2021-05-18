#include "PGameObject.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PStaticMeshComponent.hpp"
#include "PIDSequence.hpp"
#include "PScene.hpp"

PIDSequence PGameObject::m_idGenerator("GameObject");

PGameObject::PGameObject(const glm::vec3& position, const glm::quat& orientation)
{
    m_properties.previousPosition = m_properties.position = glm::vec4(position, 1.f);
    m_properties.previousScale = m_properties.scale = glm::vec3(1.f, 1.f, 1.f);
    m_properties.previousOrientation = m_properties.orientation = orientation;
    m_properties.modelMatrix = glm::mat4(1.f);
   // m_properties.renderOrder = ZRenderOrder::Static;
    id_ = m_idGenerator.Next();
    CalculateDerivedData();
}

void PGameObject::PreRender()
{
	auto scene = Scene();
	if (!scene) return;

	scene->PushMatrix(scene->TopMatrix() * m_properties.modelMatrix);
}

void PGameObject::Render(double deltaTime, const std::shared_ptr<PShader>& shader)
{
	if (std::shared_ptr<PStaticMeshComponent> graphicsComp = FindComponent<PStaticMeshComponent>())
	{
		//graphicsComp->SetGameLights(scene->GameLights());
		//graphicsComp->SetGameCamera(scene->ActiveCamera());
		graphicsComp->Render(deltaTime, shader);
	}
}

void PGameObject::RenderChildren(double deltaTime, const std::shared_ptr<PShader>& shader)
{
	std::vector<std::shared_ptr<PGameObject>>::reverse_iterator it = m_childrens.rbegin(), end = m_childrens.rend();
	for (; it != end; it++)
	{
		auto go = *it;
		go->PreRender();
		if (go->IsVisible())
		{
			go->Render(deltaTime, shader);
		}
		go->RenderChildren(deltaTime, shader);
		go->PostRender();
	}
}

void PGameObject::PostRender()
{
	auto scene = Scene();
	if (!scene) return;

	scene->PopMatrix();
}

void PGameObject::CalculateDerivedData()
{
    
    glm::mat4 translation = glm::translate(glm::mat4(1.f), glm::vec3(m_properties.position));

    m_properties.orientation = glm::normalize(m_properties.orientation);
    glm::mat4 rotation = glm::mat4_cast(m_properties.orientation);

    glm::mat4 scale = glm::scale(glm::mat4(1.f), m_properties.scale);

    m_properties.modelMatrix = translation * rotation * scale;

    //std::shared_ptr<ZGraphicsComponent> graphicsComp = FindComponent<ZGraphicsComponent>();
    //if (graphicsComp)
    //    graphicsComp->Model()->UpdateAABB(m_properties.modelMatrix);
}

void PGameObject::AddChild(std::shared_ptr<PGameObject> gameObject)
{
    if (std::find(m_childrens.begin(), m_childrens.end(), gameObject) == m_childrens.end())
    {
        if (!gameObject->Scene()) gameObject->m_sceneContext = m_sceneContext;
        if (auto parent = gameObject->Parent()) parent->RemoveChild(gameObject);
        gameObject->m_parent = shared_from_this();
        m_childrens.push_back(gameObject);
    }
}

void PGameObject::RemoveChild(std::shared_ptr<PGameObject> gameObject, bool recurse)
{
    std::vector<std::shared_ptr<PGameObject>>::iterator it = std::find(m_childrens.begin(), m_childrens.end(), gameObject);
    if (it != m_childrens.end())
    {
        gameObject->m_parent.reset();
        m_childrens.erase(it);
    }

    if (recurse)
    {
        for (auto it = m_childrens.begin(), end = m_childrens.end(); it != end; it++)
        {
            (*it)->RemoveChild(gameObject, recurse);
        }
    }
}

void PGameObject::Destroy()
{
	CleanUp();

	for (auto object : m_childrens)
		object->Destroy();
	for (auto comp : m_components)
		comp->CleanUp();
}

std::shared_ptr<PScene> PGameObject::Scene() const
{
    return m_sceneContext.lock();
}


glm::vec3 PGameObject::Position()
{
    glm::vec3 pos;
    pos = glm::vec3(m_properties.position);
    return pos;
}

glm::vec3 PGameObject::Scale()
{
    glm::vec3 scale;
    scale = m_properties.scale;
    return scale;
}

glm::quat PGameObject::Orientation()
{
    glm::quat orn;
    orn = m_properties.orientation;
    return orn;
}

glm::vec3 PGameObject::Front()
{
    glm::vec3 front;
    front = glm::conjugate(m_properties.orientation) * glm::vec3(0.f, 0.f, -1.f);
    return front;
}

glm::vec3 PGameObject::Up()
{
    glm::vec3 up;
    up = glm::conjugate(m_properties.orientation) * glm::vec3(0.f, 1.f, 0.f);
    return up;
}

glm::vec3 PGameObject::Right()
{
    glm::vec3 right;
    right = glm::conjugate(m_properties.orientation) * glm::vec3(-1.f, 0.f, 0.f);
    return right;   
}

glm::mat4 PGameObject::ModelMatrix()
{
    glm::mat4 M;
    M = m_properties.modelMatrix;
    return M;
}

glm::vec3 PGameObject::PreviousPosition()
{
    glm::vec3 prev;
    prev = glm::vec3(m_properties.previousPosition);
    return prev;
}

glm::vec3 PGameObject::PreviousFront()
{
    glm::vec3 prev;
    prev = glm::conjugate(m_properties.previousOrientation) * glm::vec3(0.f, 0.f, -1.f);
    return prev;
}

glm::vec3 PGameObject::PreviousUp()
{
    glm::vec3 prev;
    prev = glm::conjugate(m_properties.previousOrientation) * glm::vec3(0.f, 1.f, 0.f);
    return prev;
}

glm::vec3 PGameObject::PreviousRight()
{
    glm::vec3 prev;
    prev = glm::conjugate(m_properties.previousOrientation) * glm::vec3(-1.f, 0.f, 0.f);
    return prev;
}

void PGameObject::SetPosition(const glm::vec3& position)
{
    m_properties.previousPosition = m_properties.position;
    m_properties.position = glm::vec4(position, 1.f);
    CalculateDerivedData();
}

void PGameObject::SetScale(const glm::vec3& scale)
{
    m_properties.previousScale = m_properties.scale;
    m_properties.scale = scale;

    CalculateDerivedData();
}

void PGameObject::SetOrientation(const glm::quat& quaternion)
{
    m_properties.previousOrientation = m_properties.orientation;
    m_properties.orientation = quaternion;

    CalculateDerivedData();
}

void PGameObject::SetOrientation(const glm::vec3& euler)
{
    m_properties.previousOrientation = m_properties.orientation;
    m_properties.orientation = glm::quat(euler);

    CalculateDerivedData();
}

void PGameObject::SetModelMatrix(const glm::mat4& modelMatrix)
{

    m_properties.modelMatrix = modelMatrix;
}
