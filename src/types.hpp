#include <ftxui/component/component_base.hpp>
#include <memory>

typedef std::shared_ptr<ftxui::ComponentBase> baseComp;
#define vpad(i) vbox({}) | size(WIDTH, GREATER_THAN, i)
#define hpad(i) hbox({}) | size(HEIGHT, GREATER_THAN, i)
