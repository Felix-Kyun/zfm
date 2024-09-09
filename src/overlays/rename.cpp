#include "rename.hpp"
#include <cmath>
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <string>

baseComp RenameOverlay(OverlayManager &ovm, KeybindManager &kbm, Zfm &zfm) {
  using namespace ftxui;

  std::shared_ptr<bool> done = std::make_shared<bool>(0);
  std::shared_ptr<int> pos = std::make_shared<int>(0);

  InputOption opts = InputOption::Spacious();
  opts.multiline = false;
  // opts.insert = false;
  opts.cursor_position = *pos;
  opts.on_enter = [&, done] {
    ovm.closeOverlay();

    if (zfm.selectedFileName != "")
      try {
        std::filesystem::rename(zfm.currentFile(), zfm.selectedFileName);
        std::filesystem::remove(zfm.currentFile());
        zfm.refresh();

      } catch (std::filesystem::filesystem_error &err) {
        zfm.alertMessage = err.what();
        ovm.openOverlay("Alert");
      }

    *done = false;
    zfm.refocus();
    return true;
  };

  auto nameInput = Input(&zfm.selectedFileName, "Enter New Name...", opts);

  nameInput |= CatchEvent([&, done](Event e) {
    if (e == Event::Escape) {
      *done = false;
      zfm.refocus();
      return true;
    }
    return false;
  });

  auto container = Container::Vertical({
      nameInput,
  });

  return Renderer(container, [=, &ovm, &zfm] {
    if (!ovm.getOverlayState("Rename"))
      return vbox({});

    if (!*done) {
      zfm.selectedFileName = zfm.currentFile().filename();
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
