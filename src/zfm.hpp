// definations for zfm

#pragma once
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
#include <functional>

class Zfm {

private:
  std::string_view _appName{"Zfm"};
  float _version{0.1};

  ftxui::ScreenInteractive Screen = ftxui::ScreenInteractive::Fullscreen();
  BookMark bookmarks;
  Tabs tabs;
  FileManager file;
  OverlayManager overlayManager;
  KeybindManager kbm;

  baseComp fileSelector =
      ftxui::Container::Vertical({}, &file.currentSelectedFile);
  baseComp mainComponentTree = ftxui::Container::Horizontal({});
  baseComp bookMarks = ftxui::Container::Vertical({});
  baseComp fileInfo = ftxui::Container::Vertical({});
  baseComp finalTree; // assign beffore using global binds

public:
  std::string alertMessage;               // for alert overlay
  std::string selectedFileName = "Felix"; // for rename
  void quit() { Screen.ExitLoopClosure()(); }

  Zfm();

  // get the current path of the tab
  std::filesystem::path currentPath() { return tabs.getCurrentTab().path; }
  std::filesystem::path currentPath(std::filesystem::path p) {
    tabs.getCurrentTab().path = p;
    return tabs.getCurrentTab().path;
  }

  // get the full path to current file
  std::filesystem::path currentFile() {

    // refresh if the path changes
    if (currentPath() != file.currentLoadedPath)
      refresh();
    return tabs.getCurrentTab().path /
           file.currentDirectoryFiles[file.currentSelectedFile];
  }

  // navigator
  void goToPath(std::filesystem::path p);

  int width() { return Screen.dimx(); }
  int height() { return Screen.dimy(); }

  // refresh
  void refresh();

  void refocus() { fileSelector->TakeFocus(); }
};
