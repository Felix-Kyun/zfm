#pragma once 
#include "overlay.hpp"
#include "types.hpp"
#include <ftxui/component/event.hpp>
#include <functional>
#include <string>
#include <utility>
#include <vector>

class  KeybindManager {
  private: 
    std::vector<ftxui::Event> globalHandlersName;
    std::vector<std::function<bool()>> globalHandlers;
    std::vector<std::pair<std::string, ftxui::Event>> overlayHandlersName;
    std::vector<std::function<bool()>> overlayHandlers;

  public:
    void addGlobalKeybind(ftxui::Event event, std::function<bool()> f);
    void addOverlayKeybind(ftxui::Event event, std::string name, std::function<bool()> f);
    void apply(baseComp& component, OverlayManager& ovm);

};
