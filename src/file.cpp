#include "file.hpp"

std::string_view FileInfo::getFileType(std::filesystem::path p) {

  switch (this->status.type()) {
  // case std::filesystem::file_type::none:
  //     return "ERROR";
  //     break;
  // case std::filesystem::file_type::not_found:
  //     return "ERROR";
  //     break;
  case std::filesystem::file_type::regular:
    return "Regular file";
    break;
  case std::filesystem::file_type::directory:
    return "Directory";
    break;
  case std::filesystem::file_type::symlink:
    return "Symbolic link";
    break;
  case std::filesystem::file_type::block:
    return "Block device";
    break;
  case std::filesystem::file_type::character:
    return "Character device";
    break;
  case std::filesystem::file_type::fifo:
    return "IPC pipe";
    break;
  case std::filesystem::file_type::socket:
    return "IPC socket";
    break;
  case std::filesystem::file_type::unknown:
    return "Error Unknown type";
    break;
  default:
    return "ERROR";
    break;
  }
}

FileInfo File::info(std::filesystem::path p) {

  if (_cache.find(p) != _cache.end()) {
    return _cache[p];
  } else {
    _cache[p] = FileInfo(p);
    return _cache[p];
  }

  return FileInfo(p);
}
