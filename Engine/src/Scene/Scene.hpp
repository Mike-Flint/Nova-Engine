#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "Component.hpp"

class Component;

class Scene {
private:
    std::vector<std::unique_ptr<Component>> components;
    std::unordered_map<int, Component*> componentLookup;
    int nextID = 0;
    
public:
    Scene() = default;

    template<typename T>
    int AddComponent() {
        auto newComponent = std::make_unique<T>();
        newComponent->ID = nextID++;
        newComponent->GScene = this;
        
        Component* ptr = newComponent.get();
        componentLookup[ptr->ID] = ptr;
        components.push_back(std::move(newComponent));
        
        return ptr->ID;
    }

    template<typename T>
    T* GetComponent(int id) {
        auto it = componentLookup.find(id);
        if (it != componentLookup.end()) return dynamic_cast<T*>(it->second);
        return nullptr;
    }

    template<typename T>
    std::vector<T*> GetComponentsOfType() {
        std::vector<T*> result;
        for (auto& comp : components) {
            if (T* typedComp = dynamic_cast<T*>(comp.get())) {
                result.push_back(typedComp);
            }
        }
        return result;
    }
    
    void Update(float deltaTime) {
        for (auto& comp : components) comp->Update(deltaTime);
    }
    
    void Render() {
        for (auto& comp : components) comp->Render();
    }
    
    size_t GetComponentCount() const { return components.size(); }

    void Clear();
    
    size_t GetTotalComponentCount() const {
        size_t total = components.size();
        for (const auto& comp : components) {
            total += comp->GetChildCount();
        }
        return total;
    }
};