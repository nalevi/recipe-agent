include(GenerateExportHeader)

add_library(tui tui.cpp)

add_library(recipe_agent::tui ALIAS tui)

target_link_libraries(tui PRIVATE recipe_agent_options recipe_agent_warnings)

target_link_system_libraries(tui PRIVATE ftxui::screen
        ftxui::dom
        ftxui::component
        fmt::fmt
        spdlog::spdlog)

target_include_directories(tui ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(tui PUBLIC cxx_std_20)

set_target_properties(
        tui
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(tui EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/recipe_agent/tui_export.hpp)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(tui PUBLIC SAMPLE_LIBRARY_STATIC_DEFINE)
endif()