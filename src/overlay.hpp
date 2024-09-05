#pragma once
#include "types.hpp"
#include <string>

class Overlay {
public:
  std::string name;
  bool enabled = false;
  baseComp overlay;

  // ctor
  Overlay(std::string name, baseComp overlay) : name(name), overlay(overlay) {}
};

class OverlayManager {
public:
  int OverlayEnabled{};

  void addOverlay(std::string name, baseComp overlay) {
    Overlay newOverlay(name, overlay);
    Overlays.push_back(newOverlay);
  }

  baseComp getComponentTree();
  Overlay &getOverlay(std::string name);
  bool getOverlayState(std::string name);
  void openOverlay(std::string name);
  void closeOverlay();
  void toggleOverlay(std::string name);
  std::string getActiveName();

private:
  std::vector<Overlay> Overlays;
};
