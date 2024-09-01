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

using namespace std;

int main(int argc, char **argv) { Zfm zfm{}; }

Zfm::Zfm() {

  // create a new tab
  tabs.createTab("Tab 1", fs::current_path());

  // create some example bookmarks
  bookmarks.createBookMark("Home", getHomePath());
  bookmarks.createBookMark("root", fs::path("/"));
  bookmarks.createBookMark("Current", getCurrentPath());
  bookmarks.createBookMark("zkjrfhgblaeikjrhgoauerhgkajfvblakjfsbhvqoiuarweh",
                           getCurrentPath());

  // set the initial dir
  currentPath = getCurrentPath();

  // refresh variables with all the data
  refresh();

  // call the renderer
  Render();
}

void Zfm::Render() {

  using namespace ftxui;

  // bookmarks ui
  // stays static for the most part
  // unless the user adds or removes a entry

  auto bookMarks = Container::Vertical({});

  auto bookmarkButtonStyle = ButtonOption::Animated(
      Color::Default, Color::GrayDark, Color::Default, Color::White);

  for (auto &bookmark : bookmarks) {

    // shorten the name if its too large
    if (bookmark.name.size() > 10)
      bookmark.name = bookmark.name.substr(0, 7) + "...";

    bookMarks->Add(Button(
                       bookmark.name,
                       [&] {
                         // navigate to the path
                         goToPath(bookmark.path);
                       },
                       bookmarkButtonStyle) |
                   center);
  }

  auto bookmarkRenderer = Renderer(bookMarks, [&] {
    return vbox({bookMarks->Render() | vscroll_indicator |
                 size(WIDTH, ftxui::GREATER_THAN, 20) | frame}) |
           border;
  });

  // bottom file info ui
  // displays info about the currently selected file
  auto fileInfo = Container::Vertical({

  });

  auto fileInfoRenderer = Renderer(fileInfo, [&] {
    FileInfo f = file.info(fs::path(fs::current_path() / "Makefile"));

    return vbox({

               hbox({

                   // filetype
                   text(f.type), filler(),

                   // filename
                   text(f.name), filler(),

                   // size
                   text(std::to_string(f.size))

               })

           }) |
           border | size(HEIGHT, ftxui::LESS_THAN, 5);
  });

  // most complex part ???
  // render the file ui
  // but whats the difficult part ??
  // render it alone with the tabs section
  // and guess what we need to add input keyhandlers too
  // damn thats alot of work

  auto menu = Menu(currentDirectoryFiles, &currentSelectedFile);

  auto fileSelector = Container::Horizontal({
    menu,
  });

  auto fileSelectorRenderer = Renderer(fileSelector, [&] {
    return vbox({menu->Render() | flex});
  });

  // finally just glue them all together and hand it over to fxtui::screen
  auto main_component_tree =
      Container::Horizontal({// bookmarks
                             bookMarks,
                             // tabs
                             // sets current selected tab
                             // by default 0 ?

                             // file
                             // takes path from tabs[currentTab]
                             // and redners the files inside it in a menu
                             Container::Vertical({fileSelector, fileInfo})});

  auto main_renderer = Renderer(main_component_tree, [&] {
    return hbox({

               // the bookmarks menu
               bookmarkRenderer->Render(),

               //
               vbox({

                   text(this->currentPath) | center | border,
                   // tabs
                   hbox({

                   }),

                   // file selector
                   fileSelectorRenderer->Render() | flex,

                   // fileinfo
                   fileInfoRenderer->Render()

               }) | flex}) |
           flex;
  });

  Screen.Loop(main_renderer);

  // Bye, Bye!!
}

void Zfm::goToPath(fs::path p) {
  this->currentPath = p;

  // implement code to update tabs path too
  // the files renderer just renders the avaliable files in the current tab's
  // path
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

  // if (_cache.find(p) != _cache.end()) {
  //   return _cache[p];
  // } else {
  //   _cache[p] = FileInfo(p);
  //   return _cache[p];
  // }
  //
  return FileInfo(p);
}

void Zfm::refresh() {
  currentDirectoryFiles.clear();
  currentSelectedFile = 0;

  for (auto &file : fs::directory_iterator(getCurrentPath())) {
    currentDirectoryFiles.push_back(file.path().filename().string());
  }
}
