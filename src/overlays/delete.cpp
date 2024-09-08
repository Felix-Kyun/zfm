#include "delete.hpp"
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <string>

std::string makeLine(std::string fileName) {
  std::string f;

  f.append("Do you really wanna delete ");
  f.append(fileName);
  f.append(" ? 👀");

  return f;
}

baseComp DeleteOverlay(OverlayManager &ovm, KeybindManager &kbm, Zfm &zfm) {
  using namespace ftxui;

  auto confirm = Button("Confirm", [&] {
    try {
      ovm.closeOverlay();
      std::filesystem::remove(zfm.currentFile());
      zfm.refresh();
    } catch (const std::filesystem::filesystem_error &err) {
      zfm.alertMessage = err.what();
      ovm.openOverlay("Alert");
    }
  });
  auto cancel = Button("Cancel", [&] { ovm.closeOverlay(); });

  auto container = Container::Horizontal({confirm, cancel});

  kbm.addOverlayKeybind(Event::Character('q'), "Delete File", [&] {
    ovm.closeOverlay();
    return true;
  });

  return Renderer(container, [=, &ovm, &zfm] {
    if (!ovm.getOverlayState("Delete File"))
      return vbox({});

    if (!container->Focused())
      container->TakeFocus();

    return vbox({text(makeLine(zfm.currentFile().filename())) | center, hpad(1),
                 filler(),
                 hbox({confirm->Render(), vpad(2), filler(), vpad(2),
                       cancel->Render()}) |
                     center});
  });
}
