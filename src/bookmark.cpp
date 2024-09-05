#include "bookmark.hpp"

void BookMark::createBookMark(std::string name, std::filesystem::path path) {
  entries.push_back(BookMarkEntry(name, path));
}
