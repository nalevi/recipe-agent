add_subdirectory(tui)
add_subdirectory(indigents)
add_subdirectory(recipe_db)

add_executable(recipeagent
        main.cpp
)

target_link_libraries(
        recipeagent
        PRIVATE recipe_agent::recipe_agent_options
        recipe_agent::recipe_agent_warnings
        tui
        indigents
        recipedb)

target_link_system_libraries(
        recipeagent
        PRIVATE
        CLI11::CLI11
        fmt::fmt
        spdlog::spdlog
        lefticus::tools
        ftxui::screen
        ftxui::dom
        ftxui::component
        SQLiteCpp)

target_include_directories(recipeagent PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
