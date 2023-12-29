include(GenerateExportHeader)

add_library(recipedb recipe_db.cpp)

add_library(recipe_agent::recipedb ALIAS recipedb)

target_link_libraries(recipedb PRIVATE recipe_agent_options recipe_agent_warnings)

target_link_system_libraries(recipedb
        PRIVATE
        fmt::fmt
        spdlog::spdlog
        SQLiteCpp)

target_include_directories(recipedb ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(recipedb PUBLIC cxx_std_20)

set_target_properties(
        recipedb
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(recipedb EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/recipe_agent/recipedb_export.hpp)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(recipedb PUBLIC SAMPLE_LIBRARY_STATIC_DEFINE)
endif()