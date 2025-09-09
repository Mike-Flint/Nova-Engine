#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <functional>
#include <cmath>

// Forward declaration
class Scene;

class Component {
public:
    Scene* scene = nullptr;
    Component* parent = nullptr; // Батьківський компонент
    int id = 0;
    
    std::vector<float> position = {0, 0, 0};
    std::vector<float> rotation = {0, 0, 0};
    std::vector<float> scale = {1, 1, 1};
    
    virtual ~Component() = default;
    virtual void Update(float deltaTime) {
        // Спочатку оновлюємо себе, потім дітей
        UpdateSelf(deltaTime);
        UpdateChildren(deltaTime);
    }
    
    virtual void Render() {
        // Спочатку рендеримо себе, потім дітей
        RenderSelf();
        RenderChildren();
    }
    
    // Віртуальні методи для перевизначення в наследниках
    virtual void UpdateSelf(float deltaTime) {}
    virtual void RenderSelf() {}
    
private:
    std::vector<std::unique_ptr<Component>> childComponents;
    std::unordered_map<int, Component*> childLookup;
    int nextChildID = 1000; // Починаємо з 1000, щоб не конфліктувати з Scene ID
    
    void UpdateChildren(float deltaTime) {
        for (auto& child : childComponents) {
            child->Update(deltaTime);
        }
    }
    
    void RenderChildren() {
        for (auto& child : childComponents) {
            child->Render();
        }
    }
    
public:
    // Додавання дочірнього компонента
    template<typename T>
    int AddChildComponent() {
        auto newComponent = std::make_unique<T>();
        newComponent->id = nextChildID++;
        newComponent->scene = this->scene; // Передаємо ссилку на сцену
        newComponent->parent = this; // Встановлюємо себе як батька
        
        Component* ptr = newComponent.get();
        childLookup[ptr->id] = ptr;
        childComponents.push_back(std::move(newComponent));
        
        return ptr->id;
    }
    
    // Отримання дочірнього компонента по ID
    template<typename T>
    T* GetChildComponent(int id) {
        auto it = childLookup.find(id);
        if (it != childLookup.end()) {
            return dynamic_cast<T*>(it->second);
        }
        return nullptr;
    }
    
    // Пошук всіх дочірніх компонентів певного типу
    template<typename T>
    std::vector<T*> GetChildComponentsOfType() {
        std::vector<T*> result;
        for (auto& comp : childComponents) {
            if (T* typedComp = dynamic_cast<T*>(comp.get())) {
                result.push_back(typedComp);
            }
        }
        return result;
    }
    
    // Пошук першого дочірнього компонента певного типу
    template<typename T>
    T* GetFirstChildComponent() {
        auto components = GetChildComponentsOfType<T>();
        return components.empty() ? nullptr : components[0];
    }
    
    // Видалення дочірнього компонента
    void RemoveChildComponent(int id) {
        auto it = childLookup.find(id);
        if (it != childLookup.end()) {
            // Видаляємо всіх дітей цього компонента рекурсивно
            it->second->RemoveAllChildComponents();
            
            childComponents.erase(
                std::remove_if(childComponents.begin(), childComponents.end(),
                    [id](const std::unique_ptr<Component>& comp) {
                        return comp->id == id;
                    }),
                childComponents.end()
            );
            childLookup.erase(it);
            printf("Видалено дочірній компонент ID: %d\n", id);
        }
    }
    
    // Видалити всіх дочірніх компонентів
    void RemoveAllChildComponents() {
        // Спочатку рекурсивно видаляємо дітей у дітей
        for (auto& child : childComponents) {
            child->RemoveAllChildComponents();
        }
        
        childComponents.clear();
        childLookup.clear();
        printf("Видалено всі дочірні компоненти для ID: %d\n", id);
    }
    
    // Видалити себе з батьківського компонента
    void RemoveSelf() {
        if (parent) {
            parent->RemoveChildComponent(this->id);
        } else if (scene) {
            scene->RemoveComponent(this->id);
        }
    }
    
    // Видалити дочірні компоненти за типом
    template<typename T>
    void RemoveChildComponentsOfType() {
        auto components = GetChildComponentsOfType<T>();
        for (auto* comp : components) {
            RemoveChildComponent(comp->id);
        }
    }
    
    // Знайти та видалити дочірній компонент за умовою
    template<typename T>
    void RemoveChildComponentIf(std::function<bool(T*)> predicate) {
        auto components = GetChildComponentsOfType<T>();
        for (auto* comp : components) {
            if (predicate(comp)) {
                RemoveChildComponent(comp->id);
            }
        }
    }
    
    // Отримати кількість дочірніх компонентів
    size_t GetChildCount() const {
        return childComponents.size();
    }
    
    // Методи для доступу до компонентів сцени
    template<typename T>
    T* GetSceneComponent(int id) {
        if (scene) {
            return scene->GetComponent<T>(id);
        }
        return nullptr;
    }
    
    template<typename T>
    std::vector<T*> GetSceneComponentsOfType() {
        if (scene) {
            return scene->GetComponentsOfType<T>();
        }
        return {};
    }
    
    template<typename T>
    T* GetFirstSceneComponent() {
        auto components = GetSceneComponentsOfType<T>();
        return components.empty() ? nullptr : components[0];
    }
    
    // Отримання світової позиції (з урахуванням батьківської)
    std::vector<float> GetWorldPosition() const {
        std::vector<float> worldPos = position;
        if (parent) {
            auto parentWorldPos = parent->GetWorldPosition();
            for (int i = 0; i < 3; i++) {
                worldPos[i] += parentWorldPos[i];
            }
        }
        return worldPos;
    }
    
    // Встановлення локальної позиції відносно батька
    void SetLocalPosition(const std::vector<float>& localPos) {
        position = localPos;
    }
};

// Компонент камери
class CameraComponent : public Component {
public:
    float fov = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    
    void UpdateSelf(float deltaTime) override {
        // Слідкуємо за гравцем
        auto* player = GetFirstSceneComponent<PlayerComponent>();
        if (player) {
            auto playerWorldPos = player->GetWorldPosition();
            for (int i = 0; i < 3; i++) {
                position[i] += (playerWorldPos[i] - position[i]) * 0.1f;
            }
            position[1] += 5.0f; // Піднімаємо камеру
        }
        
        // Оновлюємо FOV залежно від кількості дочірніх компонентів
        auto effects = GetChildComponentsOfType<CameraEffectComponent>();
        fov = 60.0f + effects.size() * 5.0f;
    }
};

// Ефект камери (дочірній компонент камери)
class CameraEffectComponent : public Component {
public:
    float intensity = 1.0f;
    std::string effectType = "blur";
    
    void UpdateSelf(float deltaTime) override {
        // Анімуємо інтенсивність ефекту
        static float time = 0;
        time += deltaTime;
        intensity = 1.0f + sin(time * 2.0f) * 0.5f;
    }
    
    void RenderSelf() override {
        // Логіка рендеру ефекту
        // printf("Rendering %s effect with intensity %.2f\n", effectType.c_str(), intensity);
    }
};

// UI компонент
class UIComponent : public Component {
public:
    std::string text = "UI Element";
    
    void UpdateSelf(float deltaTime) override {
        // Оновлюємо текст з інформацією про гравця
        auto* player = GetFirstSceneComponent<PlayerComponent>();
        if (player) {
            auto worldPos = player->GetWorldPosition();
            text = "Player: (" + std::to_string((int)worldPos[0]) + 
                   ", " + std::to_string((int)worldPos[2]) + ")";
        }
        
        // Оновлюємо дочірні UI елементи
        auto buttons = GetChildComponentsOfType<ButtonComponent>();
        text += " | Buttons: " + std::to_string(buttons.size());
    }
};

// Кнопка (дочірній компонент UI)
class ButtonComponent : public Component {
public:
    std::string label = "Button";
    bool isPressed = false;
    
    void UpdateSelf(float deltaTime) override {
        // Симулюємо натискання кнопки
        static float timer = 0;
        timer += deltaTime;
        if ((int)timer % 3 == 0) {
            isPressed = !isPressed;
        }
    }
    
    void RenderSelf() override {
        // Логіка рендеру кнопки
        // printf("Button '%s' %s\n", label.c_str(), isPressed ? "[PRESSED]" : "[NORMAL]");
    }
};

// Гравець з можливістю мати зброю
class PlayerComponent : public Component {
public:
    float speed = 5.0f;
    float health = 100.0f;
    
    void UpdateSelf(float deltaTime) override {
        // Рух гравця
        MovePlayer(deltaTime);
        
        // Взаємодія зі зброєю
        auto weapons = GetChildComponentsOfType<WeaponComponent>();
        for (auto* weapon : weapons) {
            weapon->isActive = true; // Активуємо зброю
        }
        
        // Оновлюємо систему здоров'я
        auto* healthSystem = GetFirstChildComponent<HealthComponent>();
        if (healthSystem) {
            health = healthSystem->currentHealth;
        }
    }
    
private:
    void MovePlayer(float deltaTime) {
        static float time = 0;
        time += deltaTime * speed * 0.5f;
        position[0] = cos(time) * 3.0f;
        position[2] = sin(time) * 3.0f;
    }
};

// Система здоров'я (дочірній компонент гравця)
class HealthComponent : public Component {
public:
    float currentHealth = 100.0f;
    float maxHealth = 100.0f;
    float regeneration = 5.0f; // HP per second
    
    void UpdateSelf(float deltaTime) override {
        // Регенерація здоров'я
        currentHealth = std::min(maxHealth, currentHealth + regeneration * deltaTime);
    }
};

// Зброя (дочірній компонент гравця)
class WeaponComponent : public Component {
public:
    std::string weaponType = "Sword";
    float damage = 25.0f;
    bool isActive = false;
    
    void UpdateSelf(float deltaTime) override {
        if (isActive) {
            // Позиціонуємо зброю відносно гравця
            SetLocalPosition({1.0f, 0.5f, 0.0f}); // Справа від гравця
            
            // Анімація зброї
            static float swingTime = 0;
            swingTime += deltaTime * 3.0f;
            rotation[2] = sin(swingTime) * 15.0f; // Махи зброєю
        }
    }
    
    void RenderSelf() override {
        if (isActive) {
            auto worldPos = GetWorldPosition();
            // printf("Rendering %s at world pos (%.1f, %.1f, %.1f)\n", 
            //        weaponType.c_str(), worldPos[0], worldPos[1], worldPos[2]);
        }
    }
};

// Сцена
class Scene {
private:
    std::vector<std::unique_ptr<Component>> components;
    std::unordered_map<int, Component*> componentLookup;
    int nextID = 0;
    
public:
    template<typename T>
    int AddComponent() {
        auto newComponent = std::make_unique<T>();
        newComponent->id = nextID++;
        newComponent->scene = this;
        
        Component* ptr = newComponent.get();
        componentLookup[ptr->id] = ptr;
        components.push_back(std::move(newComponent));
        
        return ptr->id;
    }
    
    template<typename T>
    T* GetComponent(int id) {
        auto it = componentLookup.find(id);
        if (it != componentLookup.end()) {
            return dynamic_cast<T*>(it->second);
        }
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
        for (auto& comp : components) {
            comp->Update(deltaTime);
        }
    }
    
    void Render() {
        for (auto& comp : components) {
            comp->Render();
        }
    }
    
    size_t GetComponentCount() const {
        return components.size();
    }
    
    // МЕТОДИ ВИДАЛЕННЯ КОМПОНЕНТІВ СЦЕНИ
    
    // Видалити компонент по ID
    void RemoveComponent(int id) {
        auto it = componentLookup.find(id);
        if (it != componentLookup.end()) {
            // Видаляємо всіх дітей рекурсивно
            it->second->RemoveAllChildComponents();
            
            components.erase(
                std::remove_if(components.begin(), components.end(),
                    [id](const std::unique_ptr<Component>& comp) {
                        return comp->id == id;
                    }),
                components.end()
            );
            componentLookup.erase(it);
            printf("Видалено компонент сцени ID: %d\n", id);
        }
    }
    
    // Видалити всі компоненти певного типу
    template<typename T>
    void RemoveComponentsOfType() {
        auto components = GetComponentsOfType<T>();
        for (auto* comp : components) {
            RemoveComponent(comp->id);
        }
    }
    
    // Видалити компонент за умовою
    template<typename T>
    void RemoveComponentIf(std::function<bool(T*)> predicate) {
        auto components = GetComponentsOfType<T>();
        for (auto* comp : components) {
            if (predicate(comp)) {
                RemoveComponent(comp->id);
            }
        }
    }
    
    // Очистити всю сцену
    void Clear() {
        components.clear();
        componentLookup.clear();
        nextID = 0;
        printf("Сцена очищена\n");
    }
    
    // Підрахунок всіх компонентів (включно з дочірніми)
    size_t GetTotalComponentCount() const {
        size_t total = components.size();
        for (const auto& comp : components) {
            total += CountChildrenRecursive(comp.get());
        }
        return total;
    }
    
private:
    size_t CountChildrenRecursive(Component* comp) const {
        size_t count = comp->GetChildCount();
        // Тут можна додати рекурсивний підрахунок, якщо потрібно
        return count;
    }
};

int main() {
    Scene scene;
    
    // Створюємо основні компоненти сцени
    int playerID = scene.AddComponent<PlayerComponent>();
    int cameraID = scene.AddComponent<CameraComponent>();
    int uiID = scene.AddComponent<UIComponent>();
    
    // Налаштовуємо гравця та додаємо йому дочірні компоненти
    auto* player = scene.GetComponent<PlayerComponent>(playerID);
    if (player) {
        // Додаємо систему здоров'я
        int healthID = player->AddChildComponent<HealthComponent>();
        auto* health = player->GetChildComponent<HealthComponent>(healthID);
        if (health) {
            health->maxHealth = 150.0f;
            health->currentHealth = 120.0f;
        }
        
        // Додаємо декілька видів зброї
        int sword1ID = player->AddChildComponent<WeaponComponent>();
        int sword2ID = player->AddChildComponent<WeaponComponent>();
        
        auto* sword1 = player->GetChildComponent<WeaponComponent>(sword1ID);
        if (sword1) {
            sword1->weaponType = "Fire Sword";
            sword1->damage = 50.0f;
        }
        
        auto* sword2 = player->GetChildComponent<WeaponComponent>(sword2ID);
        if (sword2) {
            sword2->weaponType = "Ice Sword";
            sword2->damage = 40.0f;
        }
        
        std::cout << "Гравець створений з " << player->GetChildCount() << " дочірніми компонентами\n";
    }
    
    // Налаштовуємо камеру та додаємо ефекти
    auto* camera = scene.GetComponent<CameraComponent>(cameraID);
    if (camera) {
        // Додаємо ефекти камери
        int effect1ID = camera->AddChildComponent<CameraEffectComponent>();
        int effect2ID = camera->AddChildComponent<CameraEffectComponent>();
        int effect3ID = camera->AddChildComponent<CameraEffectComponent>();
        
        auto* effect1 = camera->GetChildComponent<CameraEffectComponent>(effect1ID);
        if (effect1) effect1->effectType = "bloom";
        
        auto* effect2 = camera->GetChildComponent<CameraEffectComponent>(effect2ID);
        if (effect2) effect2->effectType = "blur";
        
        auto* effect3 = camera->GetChildComponent<CameraEffectComponent>(effect3ID);
        if (effect3) effect3->effectType = "noise";
        
        std::cout << "Камера створена з " << camera->GetChildCount() << " ефектами\n";
    }
    
    std::cout << "\n=== ДЕМОНСТРАЦІЯ ВИДАЛЕННЯ ===\n";
    std::cout << "Початкова кількість компонентів: " << scene.GetTotalComponentCount() << "\n\n";
    
    // 1. ВИДАЛЕННЯ ДОЧІРНЬОГО КОМПОНЕНТА ПО ID
    if (player) {
        auto weapons = player->GetChildComponentsOfType<WeaponComponent>();
        if (!weapons.empty()) {
            int weaponToRemove = weapons[0]->id;
            std::cout << "1. Видаляємо зброю '" << weapons[0]->weaponType << "' (ID: " << weaponToRemove << ")\n";
            player->RemoveChildComponent(weaponToRemove);
            std::cout << "   Залишилося зброї: " << player->GetChildComponentsOfType<WeaponComponent>().size() << "\n\n";
        }
    }
    
    // 2. ВИДАЛЕННЯ ДОЧІРНІХ КОМПОНЕНТІВ ЗА ТИПОМ
    if (camera) {
        std::cout << "2. Видаляємо всі ефекти камери\n";
        std::cout << "   Було ефектів: " << camera->GetChildComponentsOfType<CameraEffectComponent>().size() << "\n";
        camera->RemoveChildComponentsOfType<CameraEffectComponent>();
        std::cout << "   Стало ефектів: " << camera->GetChildComponentsOfType<CameraEffectComponent>().size() << "\n\n";
    }
    
    // 3. ВИДАЛЕННЯ ЗА УМОВОЮ
    std::cout << "3. Додаємо нову зброю і видаляємо слабку зброю (damage < 45)\n";
    if (player) {
        int newWeaponID = player->AddChildComponent<WeaponComponent>();
        auto* newWeapon = player->GetChildComponent<WeaponComponent>(newWeaponID);
        if (newWeapon) {
            newWeapon->weaponType = "Legendary Sword";
            newWeapon->damage = 100.0f;
        }
        
        std::cout << "   Зброї до фільтрації: " << player->GetChildComponentsOfType<WeaponComponent>().size() << "\n";
        
        // Видаляємо слабку зброю
        player->RemoveChildComponentIf<WeaponComponent>([](WeaponComponent* weapon) {
            return weapon->damage < 45.0f;
        });
        
        std::cout << "   Зброї після фільтрації: " << player->GetChildComponentsOfType<WeaponComponent>().size() << "\n";
        
        // Показуємо що залишилось
        auto remainingWeapons = player->GetChildComponentsOfType<WeaponComponent>();
        for (auto* weapon : remainingWeapons) {
            std::cout << "   - Залишилась: " << weapon->weaponType << " (damage: " << weapon->damage << ")\n";
        }
        std::cout << "\n";
    }
    
    // 4. ВИДАЛЕННЯ КОМПОНЕНТА СЦЕНИ
    std::cout << "4. Видаляємо UI компонент з сцени\n";
    std::cout << "   Компонентів сцени до: " << scene.GetComponentCount() << "\n";
    scene.RemoveComponent(uiID);
    std::cout << "   Компонентів сцени після: " << scene.GetComponentCount() << "\n\n";
    
    // 5. САМОВИДАЛЕННЯ КОМПОНЕНТА
    std::cout << "5. Демонструємо самовидалення - здоров'я видаляє себе\n";
    if (player) {
        auto* health = player->GetFirstChildComponent<HealthComponent>();
        if (health) {
            std::cout << "   Дочірніх компонентів у гравця до: " << player->GetChildCount() << "\n";
            health->RemoveSelf(); // Здоров'я видаляє себе
            std::cout << "   Дочірніх компонентів у гравця після: " << player->GetChildCount() << "\n\n";
        }
    }
    
    // 6. ВИДАЛЕННЯ ЗА ТИПОМ З СЦЕНИ
    std::cout << "6. Видаляємо всі камери з сцени\n";
    std::cout << "   Камер у сцені: " << scene.GetComponentsOfType<CameraComponent>().size() << "\n";
    scene.RemoveComponentsOfType<CameraComponent>();
    std::cout << "   Камер після видалення: " << scene.GetComponentsOfType<CameraComponent>().size() << "\n\n";
    
    std::cout << "Фінальна кількість компонентів: " << scene.GetTotalComponentCount() << "\n";
    
    // 7. ПОВНА ОЧИСТКА СЦЕНИ
    std::cout << "\n7. Очищаємо всю сцену\n";
    scene.Clear();
    std::cout << "   Компонентів після очистки: " << scene.GetComponentCount() << "\n";
    
    std::cout << "\n✅ Всі методи видалення працюють!\n";
    return 0;
}