#include "tab.hpp"
#include <cmath>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/string.hpp>
#include <memory>
#include <ranges>

// creates the tab menu
void createTabMenu(OverlayManager &ovm, KeybindManager &kbm, Tabs &tab,
                   Zfm &zfm, baseComp menu) {

  using namespace ftxui;

  menu->DetachAllChildren();
  auto entries = tab.getTabNameList();

  for (auto i : std::views::iota(0, static_cast<int>(entries.size())))
    menu->Add(Button(
                  entries[i].name,
                  [&, i] {
                    tab.currentTab = i;
                    zfm.refresh();
                    ovm.closeOverlay();
                  },
                  ButtonOption::Ascii()) |
              center);

  // add new tab creation button
  menu->Add(Button(
                "+ Add New Tab",
                [&, menu] {
                  tab.createTab("Tab " + std::to_string(tab.tabSize() + 1),
                                zfm.currentPath());
                  createTabMenu(ovm, kbm, tab, zfm, menu);
                  tab.currentTab = tab.tabSize() - 1;
                  zfm.refresh();
                  ovm.closeOverlay();
                },
                ButtonOption::Ascii()) |
            center);
}

baseComp TabOverlay(OverlayManager &ovm, KeybindManager &kbm, Tabs &tab,
                    Zfm &zfm) {
  using namespace ftxui;

  std::shared_ptr<int> selected = std::make_shared<int>(tab.currentTab);

  auto menu = Container::Vertical({}, &*(selected));

  createTabMenu(ovm, kbm, tab, zfm, menu);

  kbm.addOverlayKeybind(Event::Escape, "Tab", [&] {
    ovm.closeOverlay();
    zfm.refocus();
    return true;
  });

  return Renderer(menu, [=, &ovm, &zfm] {
    if (!ovm.getOverlayState("Tab"))
      return vbox({});

    if (!menu->Focused())
      menu->TakeFocus();

    return hbox({
        vpad(1),
        vbox({
            hpad(2),
            menu->Render() | border |
                size(WIDTH, GREATER_THAN,
                     std::floor((double)zfm.width() * 0.300)) |
                size(WIDTH, LESS_THAN, std::floor((double)zfm.width() * 0.600)),
            hpad(2),
        }),
        vpad(1),
    });
  });
}
