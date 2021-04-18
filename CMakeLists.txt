cmake_minimum_required(VERSION 3.10)

project(IMGUI_GF
  LANGUAGES CXX
)

find_package(gf REQUIRED)

add_executable(example_gf
  imgui_impl_gf.cc
  example_gf/main.cc
  imgui/imgui.cpp
  imgui/imgui_demo.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
)

target_compile_options(example_gf
  PRIVATE
    "-Wall" "-Wextra"
)

target_compile_features(example_gf
  PUBLIC
    cxx_std_14
)

set_target_properties(example_gf
  PROPERTIES
    CXX_EXTENSIONS OFF
)

target_include_directories(example_gf
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(example_gf
  PRIVATE
    gf::graphics
)
