#include "Scene.hpp"
#include "Component.hpp"



void Scene::Clear() {
    components.clear();
    componentLookup.clear();
    nextID = 0;
}