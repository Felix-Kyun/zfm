#include "zfm.hpp"
#include "overlays/info.hpp"
#include "overlays/help.hpp"
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
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
  bookmarks.createBookMark("Home", getHomePath());
  bookmarks.createBookMark("root", std::filesystem::path("/"));
  bookmarks.createBookMark("Current", currentPath());
  bookmarks.createBookMark("Config", getHomePath() / ".config");

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

  // overlays

  overlayManager.addOverlay("Help", InfoOverlay(overlayManager));
  overlayManager.addOverlay("Info", HelpOverlay(overlayManager));

  // end: overlay

  auto finalTree =
      Container::Tab({main_renderer, overlayManager.getComponentTree()},
                     &overlayManager.OverlayEnabled);

  // global keybinds
  finalTree |= CatchEvent([&](Event e) {
    if (e == Event::Character('q')) {
      Screen.ExitLoopClosure()();
      return true;
    } else if (e == Event::Character('h')) {
      overlayManager.toggleOverlay("Help");
    } else if (e == Event::Character('i')) {
      overlayManager.toggleOverlay("Info");
    }

    return false;
  });
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
  currentDirectoryFiles.clear();
  currentSelectedFile = 0;
  // fileSelector->Detach();
  fileSelector->DetachAllChildren();

  currentDirectoryFiles.push_back("..");
  fileSelector->Add(Button(
      "..", [=] { goToPath(currentPath().parent_path()); },
      ButtonOption::Ascii()));

  for (auto &file : std::filesystem::directory_iterator(currentPath())) {
    string name = file.path().filename().string();
    currentDirectoryFiles.push_back(name);
    fileSelector->Add(Button(
        name, [=] { goToPath(currentPath() / name); }, ButtonOption::Ascii()));
  }

  currentLoadedPath = currentPath();
}


