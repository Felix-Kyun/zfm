// definations for zfm

// the class itself

#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;
typedef std::shared_ptr<ftxui::ComponentBase> baseComp;

class Tabs {
  class TabEntry {
  public:
    std::string name;
    fs::path path;

    TabEntry(std::string name, fs::path path) : name(name), path(path) {}
  };

private:
  std::vector<TabEntry> entries;

public:
  int currentTab{};

  TabEntry &getCurrentTab() { return entries[currentTab]; }

  void createTab(std::string name, fs::path path) {
    entries.push_back(TabEntry(name, path));
  }
};

class BookMark {
  class BookMarkEntry {
  public:
    std::string name;
    fs::path path;

    BookMarkEntry(std::string name, fs::path path) : name(name), path(path) {}
  };

private:
  std::vector<BookMarkEntry> entries;

public:
  //
  BookMarkEntry *begin() { return &entries[0]; }
  BookMarkEntry *end() { return &entries[entries.size()]; }

  //
  void createBookMark(std::string name, fs::path path) {
    entries.push_back(BookMarkEntry(name, path));
  }
};

class FileInfo {
public:
  std::string name;
  uintmax_t size{};
  std::string type;
  fs::file_status status;

  FileInfo(fs::path p) {
    this->status = fs::status(p);
    this->name = p.filename();
    this->type = getFileType(p);
    if (type != "Directory")
      this->size = fs::file_size(p);
  }

  FileInfo() {}

  std::string_view getFileType(fs::path p);
};

class File {
private:
  std::unordered_map<fs::path, FileInfo> _cache;

public:
  FileInfo info(fs::path p);
};

class Overlay {
public:
  std::string name;
  bool enabled = false;

  // ctor
  Overlay(std::string name) : name(name) {}
};

class OverlayManager {
public:
  int OverlayEnabled{};

  void addOverlay(std::string name) {
    Overlay newOverlay(name);
    Overlays.push_back(newOverlay);
  }

  Overlay &getOverlay(std::string name);
  bool getOverlayState(std::string name);
  void openOverlay(std::string name);
  void closeOverlay();
  void toggleOverlay(std::string name);
  std::string getActiveName();

private:
  std::vector<Overlay> Overlays;
};

class Zfm {

private:
  std::string_view _appName{"Zfm"};
  float _version{1.0};
  ftxui::ScreenInteractive Screen = ftxui::ScreenInteractive::Fullscreen();
  BookMark bookmarks;
  Tabs tabs;
  File file;
  fs::path currentLoadedPath; // just for validation of current directory files
  std::vector<std::string> currentDirectoryFiles;
  int currentSelectedFile = 0;
  baseComp fileSelector = ftxui::Container::Vertical({}, &currentSelectedFile);
  baseComp mainComponentTree = ftxui::Container::Horizontal({});
  baseComp bookMarks = ftxui::Container::Vertical({});
  baseComp fileInfo = ftxui::Container::Vertical({});
  OverlayManager overlayManager;

public:
  Zfm();

  // get the home dir
  fs::path getHomePath() { return fs::path(getenv("HOME")); }

  // get the current path of the tab
  fs::path currentPath() { return tabs.getCurrentTab().path; }
  fs::path currentPath(fs::path p) {
    tabs.getCurrentTab().path = p;
    return tabs.getCurrentTab().path;
  }

  // get the full path to current file
  fs::path currentFile() {

    // refresh if the path changes
    if (currentPath() != currentLoadedPath)
      refresh();
    return tabs.getCurrentTab().path /
           currentDirectoryFiles[currentSelectedFile];
  }

  // navigator
  void goToPath(fs::path p);

  FileInfo getFileInfo(fs::path p);

  // refresh
  void refresh();

  // construct and render the ui
  // void Render();
};

baseComp InfoOverlay(OverlayManager& ovm);
baseComp HelpOverlay(OverlayManager& ovm);
