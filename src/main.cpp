//
// Created by Nagy Levente on 22/12/2023.
//

#include "recipe_agent/tui/tui.h"

#include <CLI/CLI.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <memory>

#include <internal_use_only/config.hpp>


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

    auto sp_database = std::make_shared<SQLite::Database>("recipe_agent.db3");
    recipeagent::tui::display_menu();

  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }

  return 0;
}