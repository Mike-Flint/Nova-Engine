#include "Scene.hpp"
#include "Component.hpp"

template<typename T>
int Component::AddChildComponent() {
    auto newComponent = std::make_unique<T>();
    newComponent->id = nextChildID++;
    newComponent->scene = this->GScene;
    newComponent->parent = this;
    
    Component* ptr = newComponent.get();
    childLookup[ptr->id] = ptr;
    childComponents.push_back(std::move(newComponent));
    
    return ptr->id;
}

template<typename T>
T* Component::GetChildComponent(int id) {
    auto it = childLookup.find(id);
    if (it != childLookup.end()) return dynamic_cast<T*>(it->second);
    return nullptr;
}

template<typename T>
std::vector<T*> Component::GetChildComponentsOfType() {
    std::vector<T*> result;
    for (auto& comp : childComponents) {
        if (T* typedComp = dynamic_cast<T*>(comp.get())) {
            result.push_back(typedComp);
        }
    }
    return result;
}

template<typename T>
T* Component::GetSceneComponent(int id) {
    return GScene ? GScene->GetComponent<T>(id) : nullptr;
}

template<typename T>
std::vector<T*> Component::GetSceneComponentsOfType() {
    return GScene ? GScene->GetComponentsOfType<T>() : std::vector<T*>();
}