cmake_minimum_required(VERSION 3.24)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

include (FetchContent)

set (FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main)

FetchContent_MakeAvailable(ftxui) 

project(zfm LANGUAGES CXX VERSION 1.0.0)
add_definitions("-Wall" "-g")
add_executable(zfm src/zfm.cpp)
target_link_libraries(zfm 
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)


