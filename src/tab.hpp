#pragma once 
#include <vector>
#include <filesystem>
#include <string>

class Tabs {
  class TabEntry {
  public:
    std::string name;
    std::filesystem::path path;

    TabEntry(std::string name, std::filesystem::path path) : name(name), path(path) {}
  };

private:
  std::vector<TabEntry> entries;

public:
  int currentTab = 0;

  TabEntry &getCurrentTab() { return entries[currentTab]; }

  void createTab(std::string name, std::filesystem::path path) {
    entries.push_back(TabEntry(name, path));
  }

  auto getTabNameList() {
    return entries;
  }

  int tabSize() {
    return static_cast<int>(entries.size());
  }

};


