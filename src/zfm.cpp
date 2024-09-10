#include "zfm.hpp"
#include "globalKeybinds.hpp"
#include "overlays/alert.hpp"
#include "overlays/delete.hpp"
#include "overlays/help.hpp"
#include "overlays/info.hpp"
#include "overlays/new.hpp"
#include "overlays/rename.hpp"
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>

using namespace std;

int main(int argc, char **argv) { Zfm zfm{}; }

Zfm::Zfm() {
  using namespace ftxui;

  // create a new tab
  tabs.createTab("Tab 1", std::filesystem::current_path());

  // create some example bookmarks
  bookmarks.createBookMark("Home", file.getHomePath());
  bookmarks.createBookMark("root", std::filesystem::path("/"));
  bookmarks.createBookMark("Current", currentPath());
  bookmarks.createBookMark("Config", file.getHomePath() / ".config");

  // auto bookmarkButtonStyle = ButtonOption::Animated(Color::Default,
  // Color::GrayDark, Color::Default, Color::White);
  auto bookmarkButtonStyle = ButtonOption::Ascii();

  for (auto &bookmark : bookmarks) {

    // shorten the name if its too large
    if (bookmark.name.size() > 10)
      bookmark.name = bookmark.name.substr(0, 7) + "...";

    bookMarks->Add(Renderer([] { return separatorEmpty(); }));
    bookMarks->Add(Button(
                       bookmark.name,
                       [&] {
                         // navigate to the path
                         goToPath(bookmark.path);
                       },
                       bookmarkButtonStyle) |
                   center);
  }
  mainComponentTree->Add(bookMarks);
  mainComponentTree->Add(ftxui::Container::Vertical({fileSelector}));

  // refresh variables with all the data
  refresh();

  auto main_renderer = Renderer(mainComponentTree, [=] {
    return hbox({

               // the bookmarks menu
               vbox({text("Bookmarks") | center, separator(),
                     bookMarks->Render() | vscroll_indicator |
                         size(WIDTH, ftxui::GREATER_THAN, 20) | frame}) |
                   border,

               //
               vbox({

                   text(currentPath()) | center | border,
                   // tabs
                   hbox({

                   }),

                   // file selector
                   hbox({
                       vbox({}) | size(WIDTH, GREATER_THAN, 3),
                       vbox({fileSelector->Render() | vscroll_indicator |
                             frame | flex}) |
                           flex,
                   }) | flex,
                   // fileinfo

                   vbox({

                       hbox({

                           // filetype
                           text(file.info(currentFile()).type), filler(),

                           // filename
                           text(file.info(currentFile()).name), filler(),

                           // size
                           text(std::to_string(file.info(currentFile()).size))

                       })

                   }) | border |
                       size(HEIGHT, ftxui::LESS_THAN, 5)

               }) | flex}) |
           flex;
  });

  // focus on file tree initially
  fileSelector->TakeFocus();

  // overlays

  overlayManager.addOverlay("Help", InfoOverlay(overlayManager, kbm));
  overlayManager.addOverlay("Info", HelpOverlay(overlayManager, kbm));
  overlayManager.addOverlay("Delete File",
                            DeleteOverlay(overlayManager, kbm, *this));
  overlayManager.addOverlay("Alert", AlertOverlay(overlayManager, kbm, this));
  overlayManager.addOverlay("Rename",
                            RenameOverlay(overlayManager, kbm, *this));
  overlayManager.addOverlay("New", NewOverlay(overlayManager, kbm, *this));

  // end: overlay

  finalTree = Container::Tab({main_renderer, overlayManager.getComponentTree()},
                             &overlayManager.OverlayEnabled);

  // some nav binds
  kbm.addGlobalKeybind(Event::G, [&] {
    file.currentSelectedFile = file.currentDirectoryFiles.size() - 1;
    return true;
  });

  kbm.addGlobalKeybind(Event::g, [&] {
    file.currentSelectedFile = 0;
    return true;
  });

  kbm.addGlobalKeybind(Event::q, [&] {
    Screen.ExitLoopClosure()();
    return true;
  });

  kbm.addGlobalKeybind(Event::c, [&] {
    selectedFilePath = currentFile();
    mode = "copy";
    return true;
  });
  kbm.addGlobalKeybind(Event::x, [&] {
    selectedFilePath = currentFile();
    mode = "move";
    return true;
  });

  applyGlobalKeybinds(*this);

  kbm.apply(finalTree, overlayManager);

  Screen.Loop(finalTree);
}

void Zfm::goToPath(std::filesystem::path p) {

  // only navigate if the path is a directory
  if (FileInfo(p).type != "Directory")
    return;

  currentPath(p);

  // implement code to update tabs path too
  // the files renderer just renders the avaliable files in the current tab's
  // path
  refresh();
}

void Zfm::refresh() {
  using namespace ftxui;

  // Screen.Fullscreen();

  // reset every dynamic things
  file.currentDirectoryFiles.clear();
  file.currentSelectedFile = 0;
  // fileSelector->Detach();
  fileSelector->DetachAllChildren();

  file.currentDirectoryFiles.push_back("..");
  fileSelector->Add(Button(
      "..", [=] { goToPath(currentPath().parent_path()); },
      ButtonOption::Ascii()));

  for (auto &file : std::filesystem::directory_iterator(currentPath())) {
    string name = file.path().filename().string();
    this->file.currentDirectoryFiles.push_back(name);
    fileSelector->Add(Button(
        name, [=] { goToPath(currentPath() / name); }, ButtonOption::Ascii()));
  }

  file.currentLoadedPath = currentPath();
}
