#include "ContentBrowser.hpp"

void ImWindows::ContentBrowser::draw_folders(const Folder& currentFolders, Files& selectedFolders) {
    if(currentFolders.folder.empty())
        return;

    for (const Folder& entry : currentFolders.folder) {
        bool isFolderEmpty = entry.folder.empty();

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow 
                                | ImGuiTreeNodeFlags_OpenOnDoubleClick
                                | (entry.path == selectedFolders.path ? ImGuiTreeNodeFlags_Selected : 0)
                                | (isFolderEmpty ? ImGuiTreeNodeFlags_Leaf : 0);

        bool open = ImGui::TreeNodeEx(entry.path.filename().string().c_str(), flags);

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen()) {
            selectedFolders.path = entry.path;
            selectedFolders.needsUpdate = true;
        }

        if (open) {
            draw_folders(entry, selectedFolders);
            ImGui::TreePop();
        }
    }
    
}

void ImWindows::ContentBrowser::buildFolderTree(const std::filesystem::path& path, Folder& currentFolders)
{
    currentFolders.path = path;
    for (auto& entry : std::filesystem::directory_iterator(path)) {
        if (!entry.is_directory())
            continue;

        currentFolders.folder.emplace_back();
        Folder& tempFld = currentFolders.folder.back(); 

        buildFolderTree(entry.path(), tempFld);
    }
}

void ImWindows::ContentBrowser::DrawContent(){
    static Files selectedFolders(std::filesystem::current_path());
    static Folder currentFolders;
    static bool once = true;
    if (once){
        buildFolderTree(std::filesystem::path(std::filesystem::current_path()), currentFolders);
        once = false;
    }

    ImGui::BeginChild("Folders", ImVec2(200,0), true);
        draw_folders(currentFolders, selectedFolders);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Files", ImVec2(0,0), true);
        const float thumb = 64;
        const float pad = 8;
        float cell_size = thumb + pad;
        int columns = (int)(ImGui::GetContentRegionAvail().x / cell_size);
        if (columns < 1) columns = 1;
        ImGui::Columns(columns, nullptr, false);

        if(selectedFolders.needsUpdate){
            selectedFolders.needsUpdate = false;
            selectedFolders.files.clear();
            for (auto& entry : std::filesystem::directory_iterator(selectedFolders.path)) {
                if (entry.is_directory()) continue;
                selectedFolders.files.emplace_back(entry.path());
            }
        }

        for (auto& entry : selectedFolders.files) {

            std::string name = entry.filename().string();
            ImGui::Button(std::string(1, name[0]).c_str(), ImVec2(thumb, thumb));
            ImGui::TextWrapped("%s", name.c_str());
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
    ImGui::EndChild();
}
