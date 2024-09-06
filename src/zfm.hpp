// definations for zfm

#include "bookmark.hpp"
#include "file.hpp"
#include "keybind.hpp"
#include "overlay.hpp"
#include "tab.hpp"
#include "types.hpp"
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <string>

class Zfm {

private:
  std::string_view _appName{"Zfm"};
  float _version{1.0};
  ftxui::ScreenInteractive Screen = ftxui::ScreenInteractive::Fullscreen();
  BookMark bookmarks;
  Tabs tabs;
  File file;
  std::filesystem::path
      currentLoadedPath; // just for validation of current directory files
  std::vector<std::string> currentDirectoryFiles;
  int currentSelectedFile = 0;
  baseComp fileSelector = ftxui::Container::Vertical({}, &currentSelectedFile);
  baseComp mainComponentTree = ftxui::Container::Horizontal({});
  baseComp bookMarks = ftxui::Container::Vertical({});
  baseComp fileInfo = ftxui::Container::Vertical({});
  baseComp finalTree; // assign beffore using global binds
  OverlayManager overlayManager;
  KeybindManager kbm;

public:
  Zfm();

  // get the home dir
  std::filesystem::path getHomePath() {
    return std::filesystem::path(getenv("HOME"));
  }

  // get the current path of the tab
  std::filesystem::path currentPath() { return tabs.getCurrentTab().path; }
  std::filesystem::path currentPath(std::filesystem::path p) {
    tabs.getCurrentTab().path = p;
    return tabs.getCurrentTab().path;
  }

  // get the full path to current file
  std::filesystem::path currentFile() {

    // refresh if the path changes
    if (currentPath() != currentLoadedPath)
      refresh();
    return tabs.getCurrentTab().path /
           currentDirectoryFiles[currentSelectedFile];
  }

  // navigator
  void goToPath(std::filesystem::path p);

  FileInfo getFileInfo(std::filesystem::path p);

  // refresh
  void refresh();

  // construct and render the ui
  // void Render();

  // keybinds
  // defined in keybinds.cpp

};
