include(GenerateExportHeader)

add_library(indigents indigent.cpp
        indigent_manager.cpp)

add_library(recipe_agent::indigents ALIAS indigents)

target_link_libraries(indigents PRIVATE recipe_agent_options recipe_agent_warnings recipedb)

target_link_system_libraries(indigents
        PRIVATE
        fmt::fmt
        spdlog::spdlog
        SQLiteCpp)

target_include_directories(indigents ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(indigents PUBLIC cxx_std_20)

set_target_properties(
        indigents
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(indigents EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/recipe_agent/indigents_export.hpp)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(indigents PUBLIC SAMPLE_LIBRARY_STATIC_DEFINE)
endif()