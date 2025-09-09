#pragma once

#include "BaseWindow.hpp"
#include "imfilebrowser.h"

namespace ImWindows{
    class ContentBrowser : public BaseWindow{
    private:
        void DrawContent() override;

        struct Folder{
            std::filesystem::path path;
            std::vector<Folder> folder;
        };

        struct Files{
            Files(std::filesystem::path _path) : path(_path){}

            std::filesystem::path path;
            std::vector<std::filesystem::path> files;
            bool needsUpdate = true;
        };
        

        void buildFolderTree(const std::filesystem::path &path,Folder& currentFolders);
        void draw_folders(const Folder& currentFolders, Files& selectedFolders);
    public:
        ContentBrowser(EditorWindows &state, Scene* GScene) :
            BaseWindow("Content Browser", state.ContentBrowser, GScene){}
    };
};
