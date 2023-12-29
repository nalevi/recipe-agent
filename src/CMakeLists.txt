add_subdirectory(tui)
add_executable(recipeagent
        main.cpp
)

target_link_libraries(
        recipeagent
        PRIVATE recipe_agent::recipe_agent_options
        recipe_agent::recipe_agent_warnings
        tui)

target_link_system_libraries(
        recipeagent
        PRIVATE
        CLI11::CLI11
        fmt::fmt
        spdlog::spdlog
        lefticus::tools
        ftxui::screen
        ftxui::dom
        ftxui::component)

target_include_directories(recipeagent PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
