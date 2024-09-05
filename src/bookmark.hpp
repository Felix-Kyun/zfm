#pragma once
#include <filesystem>
#include <string>
#include <vector>

class BookMark {
  class BookMarkEntry {
  public:
    std::string name;
    std::filesystem::path path;

    BookMarkEntry(std::string name, std::filesystem::path path)
        : name(name), path(path) {}
  };

private:
  std::vector<BookMarkEntry> entries;

public:
  // iterators for the enteries vector
  BookMarkEntry *begin() { return &entries[0]; }
  BookMarkEntry *end() { return &entries[entries.size()]; }

  // creates a new bookmark
  void createBookMark(std::string name, std::filesystem::path path);
};
