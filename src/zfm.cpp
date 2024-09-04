#include "zfm.hpp"
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <string_view>

#define vpad(i) vbox({}) | size(WIDTH, GREATER_THAN, i)
#define hpad(i) hbox({}) | size(HEIGHT, GREATER_THAN, i)

using namespace std;

int main(int argc, char **argv) { Zfm zfm{}; }

Zfm::Zfm() {
  using namespace ftxui;

  // create a new tab
  tabs.createTab("Tab 1", fs::current_path());

  // create some example bookmarks
  bookmarks.createBookMark("Home", getHomePath());
  bookmarks.createBookMark("root", fs::path("/"));
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
      Container::Tab({main_renderer, overlayManager.getComponentTree()}, &overlayManager.OverlayEnabled);

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

void Zfm::goToPath(fs::path p) {

  // only navigate if the path is a directory
  if (FileInfo(p).type != "Directory")
    return;

  currentPath(p);

  // implement code to update tabs path too
  // the files renderer just renders the avaliable files in the current tab's
  // path
  refresh();
}

std::string_view FileInfo::getFileType(fs::path p) {
  namespace fs = std::filesystem;

  switch (this->status.type()) {
  // case fs::file_type::none:
  //     return "ERROR";
  //     break;
  // case fs::file_type::not_found:
  //     return "ERROR";
  //     break;
  case fs::file_type::regular:
    return "Regular file";
    break;
  case fs::file_type::directory:
    return "Directory";
    break;
  case fs::file_type::symlink:
    return "Symbolic link";
    break;
  case fs::file_type::block:
    return "Block device";
    break;
  case fs::file_type::character:
    return "Character device";
    break;
  case fs::file_type::fifo:
    return "IPC pipe";
    break;
  case fs::file_type::socket:
    return "IPC socket";
    break;
  case fs::file_type::unknown:
    return "Error Unknown type";
    break;
  default:
    return "ERROR";
    break;
  }
}

FileInfo File::info(fs::path p) {

  if (_cache.find(p) != _cache.end()) {
    return _cache[p];
  } else {
    _cache[p] = FileInfo(p);
    return _cache[p];
  }

  return FileInfo(p);
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

  for (auto &file : fs::directory_iterator(currentPath())) {
    string name = file.path().filename().string();
    currentDirectoryFiles.push_back(name);
    fileSelector->Add(Button(
        name, [=] { goToPath(currentPath() / name); }, ButtonOption::Ascii()));
  }

  currentLoadedPath = currentPath();
}

Overlay &OverlayManager::getOverlay(std::string name) {
  for (auto &overlay : Overlays) {
    if (overlay.name == name)
      return overlay;
  }
  return Overlays[0];
}

bool OverlayManager::getOverlayState(std::string name) {
  return getOverlay(name).enabled && OverlayEnabled;
}
void OverlayManager::openOverlay(std::string name) {
  for (auto &overlay : Overlays) {
    if (overlay.enabled)
      overlay.enabled = false;
  }

  for (auto &overlay : Overlays) {
    if (overlay.name == name)
      overlay.enabled = true;
  }

  OverlayEnabled = 1;
}
void OverlayManager::closeOverlay() {
  OverlayEnabled = 0;
  for (auto &overlay : Overlays) {
    if (overlay.enabled)
      overlay.enabled = false;
  }
}

void OverlayManager::toggleOverlay(std::string name) {
  if (getOverlay(name).enabled)
    closeOverlay();
  else
    openOverlay(name);
}

std::string OverlayManager::getActiveName() {
  for (auto &overlay : Overlays) {
    if (overlay.enabled)
      return overlay.name;
  }

  return "Undefined";
}

baseComp InfoOverlay(OverlayManager &ovm) {
  using namespace ftxui;

  auto buttonHelp = Button(
      "help", [&] { ovm.openOverlay("Help"); }, ButtonOption::Ascii());
  auto buttonClose = Button(
      "Close", [&] { ovm.closeOverlay(); }, ButtonOption::Ascii());

  auto container = Container::Horizontal({buttonHelp, buttonClose}) |
                   Maybe([&] { return ovm.getOverlayState("Info"); });

  return Renderer(container, [=, &ovm] {
    if (!ovm.getOverlayState("Info"))
      return vbox({});
    if (!container->Focused())
      buttonHelp->TakeFocus();
    return vbox({text("Howdy Hey! Welcome to Z File Manager.") | center,
                 hpad(1), filler(),
                 hbox({buttonHelp->Render(), vpad(2), filler(), vpad(2),
                       buttonClose->Render()}) |
                     center});
  });
}
baseComp HelpOverlay(OverlayManager &ovm) {
  using namespace ftxui;

  auto buttonInfo = Button(
      "Info", [&] { ovm.openOverlay("Info"); }, ButtonOption::Ascii());
  auto buttonClose = Button(
      "Close", [&] { ovm.closeOverlay(); }, ButtonOption::Ascii());

  auto container = Container::Horizontal({buttonInfo, buttonClose}) |
                   Maybe([&] { return ovm.getOverlayState("Help"); });

  return Renderer(container, [=, &ovm] {
    if (!ovm.getOverlayState("Help"))
      return vbox({});

    if (!container->Focused())
      container->TakeFocus();
    return vbox(
        {text("How may i help you when i cant help myself? :p") | center,
         hpad(1), filler(),
         hbox({buttonInfo->Render(), vpad(2), filler(), vpad(2),
               buttonClose->Render()}) |
             center});
  });
}

baseComp OverlayManager::getComponentTree() {
  using namespace ftxui;

  auto overlayBase = Container::Vertical({});

  for (auto iOverlay : Overlays) {
      overlayBase->Add(iOverlay.overlay);
  }

  auto overlay = Renderer(overlayBase, [=] {
    return hbox({vbox({text(getActiveName()) | center,
                       separator(),
                       hbox({
                           vpad(2),
                           vbox({
                               hpad(1),
                               overlayBase->Render(),
                               hpad(1),
                           }),
                           vpad(2),
                       })}) |
                 center | border}) |
           center;
  });

  return overlay;
}
