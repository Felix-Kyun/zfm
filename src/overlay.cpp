#include "overlay.hpp"
#include <ftxui/component/component.hpp>

Overlay &OverlayManager::getOverlay(std::string name) {
  for (auto &overlay : Overlays) {
    if (overlay.name == name)
      return overlay;
  }
  return Overlays[0];
}

bool OverlayManager::getOverlayState(std::string name) {
  return getOverlay(name).enabled && OverlayEnabled;
}
void OverlayManager::openOverlay(std::string name) {
  for (auto &overlay : Overlays) {
    if (overlay.enabled)
      overlay.enabled = false;
  }

  for (auto &overlay : Overlays) {
    if (overlay.name == name)
      overlay.enabled = true;
  }

  OverlayEnabled = 1;
}
void OverlayManager::closeOverlay() {
  OverlayEnabled = 0;
  for (auto &overlay : Overlays) {
    if (overlay.enabled)
      overlay.enabled = false;
  }

}

void OverlayManager::toggleOverlay(std::string name) {
  if (getOverlay(name).enabled)
    closeOverlay();
  else
    openOverlay(name);
}

std::string OverlayManager::getActiveName() {
  for (auto &overlay : Overlays) {
    if (overlay.enabled)
      return overlay.name;
  }

  return "Undefined";
}

baseComp OverlayManager::getComponentTree() {
  using namespace ftxui;

  auto overlayBase = Container::Vertical({});

  for (auto iOverlay : Overlays) {
    overlayBase->Add(iOverlay.overlay);
  }

  auto overlay = Renderer(overlayBase, [=] {
    return hbox({vbox({text(getActiveName()) | center, separator(),
                       hbox({
                           vpad(2),
                           vbox({
                               hpad(1),
                               overlayBase->Render(),
                               hpad(1),
                           }),
                           vpad(2),
                       })}) |
                 center | border}) |
           center;
  });

  return overlay;
}
