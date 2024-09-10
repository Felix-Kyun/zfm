#include "globalKeybinds.hpp"
#include <ftxui/component/event.hpp>

void applyGlobalKeybinds(Zfm &zfm) {

  using namespace ftxui;

  // add keybind

  zfm.addGlobalKeybind(Event::h, [&] {
    zfm.openOverlay("Help");
    return true;
  });

  zfm.addGlobalKeybind(Event::i, [&] {
    zfm.openOverlay("Info");
    return true;
  });

  zfm.addGlobalKeybind(Event::d, [&] {
    zfm.openOverlay("Delete File");
    return true;
  });

  zfm.addGlobalKeybind(Event::e, [&] {
    zfm.openOverlay("Alert");
    return true;
  });

  zfm.addGlobalKeybind(Event::n, [&] {
    zfm.openOverlay("New");
    return true;
  });

  zfm.addGlobalKeybind(Event::r, [&] {
    zfm.openOverlay("Rename");
    return true;
  });

}
