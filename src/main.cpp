//
// Created by Nagy Levente on 22/12/2023.
//

#include <CLI/CLI.hpp>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "ftxui/component/captured_mouse.hpp"// for ftxui
#include "ftxui/component/component.hpp"// for Radiobox, Horizontal, Menu, Renderer, Tab
#include "ftxui/component/component_base.hpp"// for ComponentBase
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <memory>

#include <internal_use_only/config.hpp>

using namespace ftxui;

ButtonOption button_style()
{
  auto option = ButtonOption::Animated();
  option.transform = [](const EntryState &s) {
    auto element = text(s.label);
    if (s.focused) { element |= bold; }
    return element | center | borderEmpty;
  };
  return option;
}

void add_indigent() { spdlog::info("Indigent successfully added!"); }

const std::vector<std::string> INDIGENT_TYPES{
  "SEED",
  "FRUIT",
  "VEGETABLE",
  "DAIRY",
  "MEAT",
  "FAT",
  "BREAD",
  "SNACK",
  "CONDIMENT",
};

int main(int argc, char **argv)
{
  try {
    CLI::App app{ fmt::format(
      "{} version {}", recipe_agent::cmake::project_name, recipe_agent::cmake::project_version) };

    bool show_version = false;
    app.add_flag("--version", show_version, "Show version information");

    CLI11_PARSE(app, argc, argv);

    if (show_version) {
      fmt::print("{}\n", recipe_agent::cmake::project_version);
      return EXIT_SUCCESS;
    }

  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }

  // Menu with tabs.
  std::vector<std::string> tab_values{
    "Indigents",
    "Recipes",
    "Generation",
  };
  int tab_selected = 0;
  auto tab_main_menu = Menu(&tab_values, &tab_selected);

  std::vector<std::string> tab_indigents_entries{
    "Add indigent",
    "Get indigent",
    "List all indigents",
    "Get indigents by category",
  };
  int tab_indigents_selected = 0;


  auto tab_indigents_menu = Menu(&tab_indigents_entries, &tab_indigents_selected);

  // Add indigents tab
  std::string indigent_name;
  auto indigent_name_field = Input(&indigent_name, "indigent name");

  int selected_indigent_type = 0;
  auto indigent_type_dropdown = Dropdown(&INDIGENT_TYPES, &selected_indigent_type);

  auto indigent_add_button = Button("Add", [&] { add_indigent(); }, button_style());

  auto add_indigents_component =
    Container::Vertical({ indigent_name_field, indigent_type_dropdown, indigent_add_button });

  auto container = Container::Horizontal({ tab_main_menu, tab_indigents_menu, add_indigents_component });

  auto renderer = Renderer(container, [&] {
    return hbox({ tab_main_menu->Render(),
             separator(),
             tab_indigents_menu->Render(),
             separator(),
             vbox({
               hbox({text("Indigent name : "), indigent_name_field->Render()}),
               hbox({text("Indigent type : "), indigent_type_dropdown->Render()}),
               indigent_add_button->Render()
             })
           })
           | border;
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);

  return 0;
}