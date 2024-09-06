#include "keybind.hpp"
#include <ftxui/component/component.hpp>
#include <utility>

void KeybindManager::addGlobalKeybind(ftxui::Event event,
                                      std::function<bool()> f) {
  globalHandlersName.push_back(event);
  globalHandlers.push_back(f);
}
void KeybindManager::addOverlayKeybind(ftxui::Event event, std::string name,
                                       std::function<bool()> f) {
  overlayHandlersName.push_back(std::make_pair(name, event));
  overlayHandlers.push_back(f);
}

void KeybindManager::apply(baseComp &component, OverlayManager& ovm) {
  using namespace ftxui;

  component |= CatchEvent([&](Event e) {
    if (!ovm.OverlayEnabled) {

      for (int i = 0; i < globalHandlersName.size(); i++) {
        if (globalHandlersName[i] == e) {
          globalHandlers[i]();
        }
      }

    } else {

      for (int i = 0; i < overlayHandlersName.size(); i++) {
        if (overlayHandlersName[i].first == ovm.getActiveName() && e == overlayHandlersName[i].second) {
          overlayHandlers[i]();
        }
      }
    }

    return false;
  });
}
