#include "info.hpp"

baseComp InfoOverlay(OverlayManager &ovm) {
  using namespace ftxui;

  auto buttonHelp = Button(
      "help", [&] { ovm.openOverlay("Help"); }, ButtonOption::Ascii());
  auto buttonClose = Button(
      "Close", [&] { ovm.closeOverlay(); }, ButtonOption::Ascii());

  auto container = Container::Horizontal({buttonHelp, buttonClose}) |
                   Maybe([&] { return ovm.getOverlayState("Info"); });

  return Renderer(container, [=, &ovm] {
    if (!ovm.getOverlayState("Info"))
      return vbox({});
    if (!container->Focused())
      buttonHelp->TakeFocus();
    return vbox({text("Howdy Hey! Welcome to Z File Manager.") | center,
                 hpad(1), filler(),
                 hbox({buttonHelp->Render(), vpad(2), filler(), vpad(2),
                       buttonClose->Render()}) |
                     center});
  });
}
