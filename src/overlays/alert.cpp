#include "alert.hpp"
#include <ftxui/component/component.hpp>

baseComp AlertOverlay(OverlayManager &ovm, KeybindManager& kbm, Zfm* zfm) {

  using namespace ftxui;
  auto quit = Button("Quit", [=] {
      zfm->quit();
      });
  auto confirm = Button("Confirm", [&] { ovm.closeOverlay(); });

  auto container = Container::Horizontal({confirm, quit});
  
  kbm.addOverlayKeybind(Event::Character('q'), "Alert", [&] {
    ovm.closeOverlay();
    return true;
  });

  return Renderer(container, [=, &ovm] {
    if (!ovm.getOverlayState("Alert"))
      return vbox({});

    if (!container->Focused())
      container->TakeFocus();

    return vbox({
      text(zfm->alertMessage) | center,
      hpad(1),
      filler(),
      hbox({confirm->Render(), vpad(2), filler(), vpad(2), quit->Render()}) |center
    });
  });

}
