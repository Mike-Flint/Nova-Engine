#include "ViewPort.hpp"

void ImWindows::ViewPort::Draw() {
    ImGui::Begin("Viewport", nullptr,
    ImGuiWindowFlags_NoMove );

        if (m_once){
            GScene->AddComponent<Camera>();
            GScene->GetComponent<Model>(GScene->AddComponent<Model>())->loadModel("\\Assets\\Object\\Sword\\Sword.obj");
        }

        

        Render.Draw();

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        ImGui::Image(
            Render.viewportFBO.TextureID, 
            viewportSize,
            ImVec2(0, 1), 
            ImVec2(1, 0)
        );

        if(Render.SelectedObjects.size() == 1){
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImVec2 windowsPos = ImGui::GetWindowPos();
            ImGuizmo::SetRect(windowsPos.x, windowsPos.y ,viewportSize.x, viewportSize.y);

            glm::mat4 cameraProjection = GScene->GetComponentsOfType<Camera>()[0]->GetProjectionMatrix();
            glm::mat4 cameraView =GScene->GetComponentsOfType<Camera>()[0]->GetViewMatrix();

            ImGuizmo::OPERATION mode;

            if (AppWindow::GetKey(GLFW_KEY_1)){
                mode = ImGuizmo::OPERATION::TRANSLATE;
            }else if(AppWindow::GetKey(GLFW_KEY_2)){
                mode = ImGuizmo::OPERATION::ROTATE;
            }else if(AppWindow::GetKey(GLFW_KEY_3)){
                mode = ImGuizmo::OPERATION::SCALE;
            }else {
                mode = ImGuizmo::OPERATION::TRANSLATE;
            }

            for(Mesh& mesh : GScene->GetComponentsOfType<Model>()[0]->Meshes){
                if(mesh.VAO.ID == Render.SelectedObjects[0]){
                    glm::mat4 transform = mesh.model;

                    ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                                        mode, ImGuizmo::LOCAL, glm::value_ptr(transform));

                    if(ImGuizmo::IsUsing()){

                        glm::vec3 scale, translation, skew;
                        glm::vec4 perspective;
                        glm::quat orientation;

                        glm::decompose(transform ,scale, orientation, translation, skew, perspective);
                        mesh.model = transform;

                        mesh.position = translation;
                        mesh.rotation = glm::eulerAngles(orientation);
                        mesh.scale = scale;
                    }
                }   
            }
        }

        if(!ImGuizmo::IsUsing())
            selectMeshUnderCursor();

        

    ImGui::End();
    
    if (m_once){
            m_once = false;
            ImGuiID dockspace_id = ImGui::GetID("DockSpace");
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

            ImGui::DockBuilderDockWindow("Viewport", dockspace_id);
            ImGui::DockBuilderFinish(dockspace_id);
        }
    
}



glm::vec3 GetRayFromMouse(float mouseX, float mouseY, glm::ivec2 viewportSize,
                            const glm::mat4& view, const glm::mat4& projection,
                            const glm::vec3& cameraPos)
{
    float ndcX = (2.0f * mouseX) / viewportSize.x - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY) / viewportSize.y;

    glm::vec4 rayClip(ndcX, ndcY, -1.0f, 1.0f);
    glm::mat4 invProj = glm::inverse(projection);
    glm::vec4 rayEye = invProj * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::mat4 invView = glm::inverse(view);
    glm::vec4 rayWorld4 = invView * rayEye;
    glm::vec3 rayDir = glm::normalize(glm::vec3(rayWorld4));

    return rayDir;
}

std::optional<float> RayTriangleIntersect(
    const glm::vec3& rayOrigin,
    const glm::vec3& rayDir,
    const glm::vec3& v0,
    const glm::vec3& v1,
    const glm::vec3& v2)
{
    const float EPSILON = 1e-6f;
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(rayDir, edge2);
    float a = glm::dot(edge1, h);
    if (fabs(a) < EPSILON) return std::nullopt;

    float f = 1.0f / a;
    glm::vec3 s = rayOrigin - v0;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f) return std::nullopt;

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(rayDir, q);
    if (v < 0.0f || u + v > 1.0f) return std::nullopt;

    float t = f * glm::dot(edge2, q);
    if (t > EPSILON) return t;
    return std::nullopt;
}

void ImWindows::ViewPort::selectMeshUnderCursor(){
    if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()){
        ImVec2 winSize = ImGui::GetWindowSize();
        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 mousePos = ImGui::GetMousePos();

        glm::vec3 rayOrigin = GScene->GetComponentsOfType<Camera>()[0]->Position; // Get camera position
        glm::vec3 rayDir = GetRayFromMouse(  mousePos.x - winPos.x, mousePos.y - winPos.y, 
                                        glm::ivec2(winSize.x, winSize.y),
                                        GScene->GetComponentsOfType<Camera>()[0]->GetViewMatrix(), GScene->GetComponentsOfType<Camera>()[0]->GetProjectionMatrix(),
                                        rayOrigin); // Get ray direction

        float closestT = FLT_MAX;
        int pickedTriangle = -1;
        bool missСlick = true;

        for (auto& mesh : GScene->GetComponentsOfType<Model>()[0]->Meshes) {
            for (size_t i = 0; i < mesh.Indices.size(); i += 3) {
                glm::vec3 v0 = glm::vec3(mesh.model * glm::vec4(mesh.Vertices[mesh.Indices[i + 0]].Position, 1.0f));
                glm::vec3 v1 = glm::vec3(mesh.model * glm::vec4(mesh.Vertices[mesh.Indices[i + 1]].Position, 1.0f));
                glm::vec3 v2 = glm::vec3(mesh.model * glm::vec4(mesh.Vertices[mesh.Indices[i + 2]].Position, 1.0f));

                auto hit = RayTriangleIntersect(rayOrigin, rayDir, v0, v1, v2);

                if (hit && *hit < closestT) {
                    missСlick = false;
                    if(ImGui::IsKeyDown(ImGuiKey_LeftShift)){
                        closestT = *hit;
                        pickedTriangle = (int)i / 3;

                        for(GLuint id : Render.SelectedObjects){
                            if (id == mesh.VAO.ID){
                                break;
                            }
                        }

                        Render.SelectedObjects.emplace_back(mesh.VAO.ID);
                    }else{
                        closestT = *hit;
                        pickedTriangle = (int)i / 3;
                        Render.SelectedObjects.clear();
                        Render.SelectedObjects.emplace_back(mesh.VAO.ID);
                    }
                }
            }
        }
        if(missСlick){
            Render.SelectedObjects.clear();
        }
        // if (pickedTriangle != -1) {
        //     std::cout << "Selected tringle #" << pickedTriangle 
        //             << " at a distance " << closestT << "\n";
        // }
    }
}