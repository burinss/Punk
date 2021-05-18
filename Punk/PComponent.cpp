#include "PComponent.hpp"
#include "PStaticMeshComponent.hpp"
#include "PGameObject.hpp"
std::shared_ptr<PComponent> PComponent::CreateMeshComponent(const std::shared_ptr<PGameObject>& gameObject)
{
    std::shared_ptr<PStaticMeshComponent> comp=std::make_shared<PStaticMeshComponent>();
    if(gameObject)gameObject->AddComponent(comp);
    return comp;
}
