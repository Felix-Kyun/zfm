#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>

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

class File {
private:
  std::unordered_map<std::filesystem::path, FileInfo> _cache;

public:
  FileInfo info(std::filesystem::path p);
};
