//
// Created by Nagy Levente on 29/12/2023.
//
#include "recipe_agent/tui/tui.h"
#include "recipe_agent/indigents/indigent.h"
#include "recipe_agent/recipe_db/recipe_db.h"
#include <ftxui/component/loop.hpp>

#include <spdlog/spdlog.h>


namespace recipeagent {
namespace tui {

  IndigentsMenu::IndigentsMenu(const std::shared_ptr<SQLite::Database> &db)
    : tab_indigents_entries({
        "Add indigent",
        "Get indigent",
        "List all indigents",
        "Get indigents by category",
      }),
      indigent_name_field(), indigent_name_input(ftxui::Input(&indigent_name_field, "name here")),
      selected_indigent_type(0),
      indigent_type_dropdown(ftxui::Dropdown(&indigent::INDIGENT_TYPES, &selected_indigent_type)),
      indigent_add_button(ftxui::Button(
        "Add",
        [&] {
          indigent_manager->add_indigent(
            { indigent_name_field, static_cast<indigent::IndigentType>(selected_indigent_type) });
        },
        button_style())),
      tab_indigents_selected(0), indigent_manager(std::make_unique<recipeagent::indigent::IndigentManager>(db))
  {}

  void display_menu(const std::shared_ptr<spdlog::logger> &logger)
  {
    using namespace ftxui;

    auto sp_database = recipeagent::database::create_db_connection("recipe_agent.db3", logger);

    std::vector<std::string> tab_values{
      "Indigents",
      "Recipes",
      "Generation",
    };
    int tab_selected = 0;
    auto tab_main_menu = Menu(&tab_values, &tab_selected);

    auto indigents_menu = IndigentsMenu{ sp_database };
    auto tab_indigents_menu = display_indigents_menu(indigents_menu);
    auto add_indigents_component = create_indigents_component(indigents_menu);

    auto container = Container::Horizontal({ tab_main_menu, tab_indigents_menu, add_indigents_component });

    auto renderer = Renderer(container, [&] {
      return hbox({ tab_main_menu->Render(),
               separator(),
               tab_indigents_menu->Render(),
               separator(),
               vbox({ hbox({ text("Indigent name : "), indigents_menu.indigent_name_input->Render() }),
                 hbox({ text("Indigent type : "), indigents_menu.indigent_type_dropdown->Render() }),
                 indigents_menu.indigent_add_button->Render() }) })
             | border;
    });

    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);
  }

  ftxui::Component display_indigents_menu(IndigentsMenu &indigents_menu)
  {
    using namespace ftxui;

    return Menu(&indigents_menu.tab_indigents_entries, &indigents_menu.tab_indigents_selected);
  }

  ftxui::Component create_indigents_component(IndigentsMenu &indigents_menu)
  {
    using namespace ftxui;

    return Container::Vertical({ indigents_menu.indigent_name_input,
      indigents_menu.indigent_type_dropdown,
      indigents_menu.indigent_add_button });
  }

  ftxui::ButtonOption button_style()
  {
    using namespace ftxui;

    auto option = ButtonOption::Animated();
    option.transform = [](const EntryState &s) {
      auto element = text(s.label);
      if (s.focused) { element |= bold; }
      return element | center | borderEmpty;
    };

    return option;
  }


}// namespace tui
}// namespace recipeagent