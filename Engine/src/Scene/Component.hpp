#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

class Scene;

class Component {
public:
    Scene* GScene = nullptr;

    int ID = 0;
    
    glm::vec3 Position = {0, 0, 0};
    glm::vec3 Rotation = {0, 0, 0};
    glm::vec3 Scale = {1, 1, 1};

    glm::mat4 model = glm::mat4(1.0f);

    std::vector<std::unique_ptr<Component>> childComponents;
    std::unordered_map<int, Component*> childLookup;
    
    virtual ~Component() = default;
    
    virtual void Update(float deltaTime) {
        UpdateSelf(deltaTime);
        UpdateChildren(deltaTime);
    }
    
    virtual void Render() {
        RenderSelf();
        RenderChildren();
    }
    
    virtual void UpdateSelf(float deltaTime) {}
    virtual void RenderSelf() {}
    
    // --- Методи для дочірніх компонентів (не залежать від інших) ---
    template<typename T>
    int AddChildComponent();
    
    template<typename T>
    T* GetChildComponent(int id);
    
    template<typename T>
    std::vector<T*> GetChildComponentsOfType();
    
    size_t GetChildCount() const { return childComponents.size(); }
    
    // --- Методи для доступу до сцени (реалізація ПІСЛЯ Scene) ---
    template<typename T> 
    T* GetSceneComponent(int id);

    template<typename T> 
    std::vector<T*> GetSceneComponentsOfType();
    
    std::vector<float> GetWorldPosition() const {
        // std::vector<float> worldPos = position;
        // if (!components.empty()) {
        //     for (auto component : components){
        //         worldPos[i] += parentWorldPos[i];
            
        //         auto parentWorldPos = parent->GetWorldPosition();
        //         for (int i = 0; i < 3; i++) {
        //             worldPos[i] += parentWorldPos[i];
        //         }
        //     }
        // }
        // return worldPos;
    }
    
    void SetLocalPosition(const glm::vec3& localPos) {
        Position = localPos;
    }
private:
    
    int nextChildID = 1000;
    
    void UpdateChildren(float deltaTime) {
        for (auto& child : childComponents) child->Update(deltaTime);
    }
    
    void RenderChildren() {
        for (auto& child : childComponents) child->Render();
    }
};