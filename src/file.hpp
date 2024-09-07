#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

class FileInfo {
public:
  std::string name;
  uintmax_t size{};
  std::string type;
  std::filesystem::file_status status;

  FileInfo(std::filesystem::path p) {
    this->status = std::filesystem::status(p);
    this->name = p.filename();
    this->type = getFileType(p);
    if (type != "Directory")
      this->size = std::filesystem::file_size(p);
  }

  FileInfo() {}

  std::string_view getFileType(std::filesystem::path p);
};

class FileManager {
private:
  std::unordered_map<std::filesystem::path, FileInfo> _cache;

public:
  int currentSelectedFile = 0;
  std::filesystem::path currentLoadedPath;
  std::vector<std::string> currentDirectoryFiles;
  FileInfo info(std::filesystem::path p);

  // get the home dir
  std::filesystem::path getHomePath() {
    return std::filesystem::path(getenv("HOME"));
  }

  FileInfo getFileInfo(std::filesystem::path p);
};
