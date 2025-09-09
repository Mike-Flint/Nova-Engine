#pragma once

struct EditorWindowConfig {

    bool Show = false;
    bool Dock = true;

    bool NeedsSettingsDock;
};

struct EditorWindows{
    EditorWindows() = default;

    EditorWindowConfig ContentBrowser;
    EditorWindowConfig DetailsPanel;
    EditorWindowConfig Hierarchy;

    bool CanResize = true;
};