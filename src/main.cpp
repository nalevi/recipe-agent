//
// Created by Nagy Levente on 22/12/2023.
//

#include "recipe_agent/recipe_db/recipe_db.h"
#include "recipe_agent/tui/tui.h"

#include <CLI/CLI.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <fmt/core.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <memory>

#include <internal_use_only/config.hpp>

auto LOGGER = spdlog::basic_logger_st("recipe-agent", "recipe_agent.log");

void set_log_level(int level)
{
  switch (level) {
  case 0:
    LOGGER->set_level(spdlog::level::level_enum::trace);
    break;
  case 1:
    LOGGER->set_level(spdlog::level::level_enum::debug);
    break;
  case 2:
    LOGGER->set_level(spdlog::level::level_enum::info);
    break;
  case 3:
    LOGGER->set_level(spdlog::level::level_enum::warn);
    break;
  case 4:
    LOGGER->set_level(spdlog::level::level_enum::err);
    break;
  case 5:
    LOGGER->set_level(spdlog::level::level_enum::critical);
    break;
  case 6:
    LOGGER->set_level(spdlog::level::level_enum::off);
    break;
  }
}

int main(int argc, char **argv)
{
  try {
    CLI::App app{ fmt::format(
      "{} version {}", recipe_agent::cmake::project_name, recipe_agent::cmake::project_version) };

    bool show_version = false;
    app.add_flag("--version", show_version, "Show version information");

    int log_level = 1;// debug level
    app.add_flag("--loglevel", log_level, "Set Log Level (0-6), where 0 is TRACE and 6 is OFF. Default is 1 (debug)");

    CLI11_PARSE(app, argc, argv);

    set_log_level(log_level);

    if (show_version) {
      fmt::print("{}\n", recipe_agent::cmake::project_version);
      return EXIT_SUCCESS;
    }
    
    recipeagent::tui::display_menu(LOGGER);

  } catch (const std::exception &e) {
    LOGGER->error("Unhandled exception in main: {}", e.what());
  }

  return 0;
}