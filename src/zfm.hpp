// definations for zfm

// the class itself

#include <filesystem>
#include <ftxui/component/screen_interactive.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

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

  TabEntry getCurrentTab() { return entries[currentTab]; }

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
  uintmax_t size;
  std::string type;
  fs::file_status status;

  FileInfo(fs::path p) {
    this->status = fs::status(p);
    this->name = p.filename();
    this->type = getFileType(p);
    this->size = fs::file_size(p);
  }

  std::string_view getFileType(fs::path p);
};

class File {
private:
  std::unordered_map<fs::path, FileInfo> _cache;

public:
  FileInfo info(fs::path p);
};

class Zfm {

private:
  std::string_view _appName{"Zfm"};
  float _version{1.0};
  ftxui::ScreenInteractive Screen = ftxui::ScreenInteractive::Fullscreen();
  BookMark bookmarks;
  Tabs tabs;
  fs::path currentPath;
  std::string currentFile;
  File file;
  std::vector<std::string> currentDirectoryFiles;
  int currentSelectedFile;

public:
  Zfm();

  // get the home dir
  fs::path getHomePath() { return fs::path(getenv("HOME")); }

  // get the current path of the tab
  fs::path getCurrentPath() { return tabs.getCurrentTab().path; }

  // get the full path to current file
  fs::path getCurrentFile() { return currentPath / currentFile; }

  // navigator
  void goToPath(fs::path p);

  FileInfo getFileInfo(fs::path p);

  // refresh
  void refresh();

  // construct and render the ui
  void Render();
};

