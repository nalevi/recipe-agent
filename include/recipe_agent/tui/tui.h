//
// Created by Nagy Levente on 29/12/2023.
//

#ifndef RECIPE_AGENT_TUI_H
#define RECIPE_AGENT_TUI_H

#include "recipe_agent/indigents/indigent_manager.h"

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include <ftxui/component/loop.hpp>

#include "SQLiteCpp/Database.h"

#include <spdlog/spdlog.h>

#include <memory>
#include <vector>

namespace recipeagent {

namespace tui {

  /// @brief This object encapsulates the tab of the Indigents menu with all its fields.
  /// Also, keeps track of the selected fields.
  struct IndigentsMenu
  {
    explicit IndigentsMenu(const std::shared_ptr<SQLite::Database> &db);

    std::vector<std::string> tab_indigents_entries;

    std::string indigent_name_field;
    ftxui::Component indigent_name_input;

    int selected_indigent_type;
    ftxui::Component indigent_type_dropdown;

    ftxui::Component indigent_add_button;

    int tab_indigents_selected;
    std::unique_ptr<recipeagent::indigent::IndigentManager> indigent_manager;
  };

  /// @brief This function is the main entry point of the tui module.
  void display_menu(const std::shared_ptr<spdlog::logger> &logger);

  /// @brief This function is the entry point to draw all the indigent related menus, tabs, etc.
  ftxui::Component display_indigents_menu(IndigentsMenu &indigents_menu);

  ftxui::Component create_indigents_component(IndigentsMenu &indigents_menu);

  ftxui::ButtonOption button_style();

}// namespace tui

}// namespace recipeagent
#endif// RECIPE_AGENT_TUI_H
