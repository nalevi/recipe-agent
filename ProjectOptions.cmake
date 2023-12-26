include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(recipe_agent_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(recipe_agent_setup_options)
  option(recipe_agent_ENABLE_HARDENING "Enable hardening" ON)
  option(recipe_agent_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    recipe_agent_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    recipe_agent_ENABLE_HARDENING
    OFF)

  recipe_agent_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR recipe_agent_PACKAGING_MAINTAINER_MODE)
    option(recipe_agent_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(recipe_agent_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(recipe_agent_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(recipe_agent_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(recipe_agent_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(recipe_agent_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(recipe_agent_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(recipe_agent_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(recipe_agent_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(recipe_agent_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(recipe_agent_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(recipe_agent_ENABLE_PCH "Enable precompiled headers" OFF)
    option(recipe_agent_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(recipe_agent_ENABLE_IPO "Enable IPO/LTO" ON)
    option(recipe_agent_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(recipe_agent_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(recipe_agent_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(recipe_agent_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(recipe_agent_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(recipe_agent_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(recipe_agent_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(recipe_agent_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(recipe_agent_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(recipe_agent_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(recipe_agent_ENABLE_PCH "Enable precompiled headers" OFF)
    option(recipe_agent_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      recipe_agent_ENABLE_IPO
      recipe_agent_WARNINGS_AS_ERRORS
      recipe_agent_ENABLE_USER_LINKER
      recipe_agent_ENABLE_SANITIZER_ADDRESS
      recipe_agent_ENABLE_SANITIZER_LEAK
      recipe_agent_ENABLE_SANITIZER_UNDEFINED
      recipe_agent_ENABLE_SANITIZER_THREAD
      recipe_agent_ENABLE_SANITIZER_MEMORY
      recipe_agent_ENABLE_UNITY_BUILD
      recipe_agent_ENABLE_CLANG_TIDY
      recipe_agent_ENABLE_CPPCHECK
      recipe_agent_ENABLE_COVERAGE
      recipe_agent_ENABLE_PCH
      recipe_agent_ENABLE_CACHE)
  endif()

  recipe_agent_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (recipe_agent_ENABLE_SANITIZER_ADDRESS OR recipe_agent_ENABLE_SANITIZER_THREAD OR recipe_agent_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(recipe_agent_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(recipe_agent_global_options)
  if(recipe_agent_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    recipe_agent_enable_ipo()
  endif()

  recipe_agent_supports_sanitizers()

  if(recipe_agent_ENABLE_HARDENING AND recipe_agent_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR recipe_agent_ENABLE_SANITIZER_UNDEFINED
       OR recipe_agent_ENABLE_SANITIZER_ADDRESS
       OR recipe_agent_ENABLE_SANITIZER_THREAD
       OR recipe_agent_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${recipe_agent_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${recipe_agent_ENABLE_SANITIZER_UNDEFINED}")
    recipe_agent_enable_hardening(recipe_agent_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(recipe_agent_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(recipe_agent_warnings INTERFACE)
  add_library(recipe_agent_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  recipe_agent_set_project_warnings(
    recipe_agent_warnings
    ${recipe_agent_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(recipe_agent_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(recipe_agent_options)
  endif()

  include(cmake/Sanitizers.cmake)
  recipe_agent_enable_sanitizers(
    recipe_agent_options
    ${recipe_agent_ENABLE_SANITIZER_ADDRESS}
    ${recipe_agent_ENABLE_SANITIZER_LEAK}
    ${recipe_agent_ENABLE_SANITIZER_UNDEFINED}
    ${recipe_agent_ENABLE_SANITIZER_THREAD}
    ${recipe_agent_ENABLE_SANITIZER_MEMORY})

  set_target_properties(recipe_agent_options PROPERTIES UNITY_BUILD ${recipe_agent_ENABLE_UNITY_BUILD})

  if(recipe_agent_ENABLE_PCH)
    target_precompile_headers(
      recipe_agent_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(recipe_agent_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    recipe_agent_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(recipe_agent_ENABLE_CLANG_TIDY)
    recipe_agent_enable_clang_tidy(recipe_agent_options ${recipe_agent_WARNINGS_AS_ERRORS})
  endif()

  if(recipe_agent_ENABLE_CPPCHECK)
    recipe_agent_enable_cppcheck(${recipe_agent_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(recipe_agent_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    recipe_agent_enable_coverage(recipe_agent_options)
  endif()

  if(recipe_agent_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(recipe_agent_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(recipe_agent_ENABLE_HARDENING AND NOT recipe_agent_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR recipe_agent_ENABLE_SANITIZER_UNDEFINED
       OR recipe_agent_ENABLE_SANITIZER_ADDRESS
       OR recipe_agent_ENABLE_SANITIZER_THREAD
       OR recipe_agent_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    recipe_agent_enable_hardening(recipe_agent_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
