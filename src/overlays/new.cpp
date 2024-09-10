#include "new.hpp"
#include <cmath>
#include <filesystem>
#include <fstream>

baseComp NewOverlay(OverlayManager &ovm, KeybindManager &kbm, Zfm &zfm) {
  using namespace ftxui;

  std::shared_ptr<bool> done = std::make_shared<bool>(0);

  InputOption opts = InputOption::Spacious();
  opts.multiline = false;
  // opts.insert = false;
  opts.on_enter = [&, done] {
    ovm.closeOverlay();
    *done = false;
    zfm.refocus();

    if (zfm.selectedFileName != "")
      try {
        std::filesystem::path filePath{zfm.selectedFileName};
        filePath = zfm.currentPath() / filePath;
        std::filesystem::create_directories(filePath.parent_path());

        // handle new file creation here
        if (zfm.selectedFileName[zfm.selectedFileName.size() - 1] == '/') {
          // create a directory
          std::filesystem::create_directory(filePath);

        } else {
          // create a file in the directory
          std::ofstream file{filePath};
          file.close();
        }

        zfm.refresh();

      } catch (std::filesystem::filesystem_error &err) {
        zfm.alertMessage = err.what();
        ovm.openOverlay("Alert");
      }

    return true;
  };

  auto nameInput = Input(&zfm.selectedFileName, "Enter File Name...", opts);

  kbm.addOverlayKeybind(Event::Escape, "New", [&, done] {
    ovm.closeOverlay();
    *done = false;
    zfm.refocus();
    return true;
  });

  auto container = Container::Vertical({
      nameInput,
  });

  return Renderer(container, [=, &ovm, &zfm] {
    if (!ovm.getOverlayState("New"))
      return vbox({});

    if (!*done) {
      zfm.selectedFileName = "";
      container->TakeFocus();
      *done = true;
    }

    return hbox({
        vpad(1),
        vbox({
            hpad(2),
            nameInput->Render() | border |
                size(WIDTH, GREATER_THAN,
                     std::floor((double)zfm.width() * 0.300)) |
                size(WIDTH, LESS_THAN, std::floor((double)zfm.width() * 0.600)),
            hpad(2),
        }),
        vpad(1),
    });
  });
}
