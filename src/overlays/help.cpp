#include "help.hpp"

baseComp HelpOverlay(OverlayManager &ovm, KeybindManager& kbm) {
  using namespace ftxui;

  auto buttonInfo = Button(
      "Info", [&] { ovm.openOverlay("Info"); }, ButtonOption::Ascii());
  auto buttonClose = Button(
      "Close", [&] { ovm.closeOverlay(); }, ButtonOption::Ascii());

  auto container = Container::Horizontal({buttonInfo, buttonClose}) |
                   Maybe([&] { return ovm.getOverlayState("Help"); });

  // register keybinds
  kbm.addOverlayKeybind(Event::Character('q'), "Help", [&] {
    ovm.closeOverlay();
    return true;
  });

  return Renderer(container, [=, &ovm] {
    if (!ovm.getOverlayState("Help"))
      return vbox({});

    if (!container->Focused())
      container->TakeFocus();
    return vbox(
        {text("How may i help you when i cant help myself? :p") | center,
         hpad(1), filler(),
         hbox({buttonInfo->Render(), vpad(2), filler(), vpad(2),
               buttonClose->Render()}) |
             center});
  });
}
