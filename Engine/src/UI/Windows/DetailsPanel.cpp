#include "DetailsPanel.hpp"

void ImWindows::DetailsPanel::DrawContent(){

    // if(renderer->SelectedObjects.size() == 1){   
    //     for(Mesh& meshe : GScene->GetComponentsOfType<Model>()[0]->Meshes){
    //         if(meshe.VAO.ID == (renderer->SelectedObjects)[0]){
    //             bool changed = false;

    //             glm::vec3& position = meshe.position;
    //             glm::vec3 rotation = glm::degrees(meshe.rotation);
    //             glm::vec3& scale = meshe.scale;

    //             ImGui::Text("Transform");
    //             ImGui::SetCursorPosX(30.0f);
    //             if (ImGui::BeginTable("Transform", 2, ImGuiTableFlags_SizingStretchProp)) {
    //                 ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 60.0f);
    //                 ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

    //                 ImGui::TableNextRow();
    //                 ImGui::TableSetColumnIndex(0); ImGui::Text("X");
    //                 ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##X", &position.x, 0.1f);

    //                 ImGui::TableNextRow();
    //                 ImGui::TableSetColumnIndex(0); ImGui::Text("Y");
    //                 ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##Y", &position.y, 0.1f);

    //                 ImGui::TableNextRow();
    //                 ImGui::TableSetColumnIndex(0); ImGui::Text("Z");
    //                 ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##Z", &position.z, 0.1f);

    //                 ImGui::EndTable();
    //             }

    //             ImGui::Text("Rotation");
    //             ImGui::SetCursorPosX(30.0f);
    //             if (ImGui::BeginTable("Rotation", 2, ImGuiTableFlags_SizingStretchProp)) {
    //                 ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 60.0f);
    //                 ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

    //                 ImGui::TableNextRow();
    //                 ImGui::TableSetColumnIndex(0); ImGui::Text("Pitch");
    //                 ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##Pitch", &rotation.x, 0.1f);

    //                 ImGui::TableNextRow();
    //                 ImGui::TableSetColumnIndex(0); ImGui::Text("Yaw");
    //                 ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##Yaw", &rotation.y, 0.1f);

    //                 ImGui::TableNextRow();
    //                 ImGui::TableSetColumnIndex(0); ImGui::Text("Roll");
    //                 ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##Roll", &rotation.z, 0.1f);

    //                 ImGui::EndTable();
    //             }

    //             ImGui::Text("Scale");
    //             ImGui::SetCursorPosX(30.0f);
    //             if (ImGui::BeginTable("Scale", 2, ImGuiTableFlags_SizingStretchProp)) {
    //                 ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 60.0f);
    //                 ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

    //                 ImGui::TableNextRow();
    //                 ImGui::TableSetColumnIndex(0); ImGui::Text("Width");
    //                 ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##Width", &scale.x, 0.1f);

    //                 ImGui::TableNextRow();
    //                 ImGui::TableSetColumnIndex(0); ImGui::Text("Height");
    //                 ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##Height", &scale.y, 0.1f);

    //                 ImGui::TableNextRow();
    //                 ImGui::TableSetColumnIndex(0); ImGui::Text("Depth");
    //                 ImGui::TableSetColumnIndex(1); changed |= ImGui::DragFloat("##Depth", &scale.z, 0.1f);

    //                 ImGui::EndTable();
    //             }
    //             if(changed){
    //                 meshe.rotation = glm::radians(rotation);
    //                 meshe.model = glm::translate(glm::mat4(1.0f), position)
    //                                 * glm::toMat4(glm::quat(meshe.rotation))
    //                                 * glm::scale(glm::mat4(1.0f), scale);
    //             }
    //         }
    //     }
    // }
}