cmake_minimum_required(VERSION 3.10.2)

project(algorithms)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS true)

# Enable warnings


if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory(basic)
add_subdirectory(data_structures)
add_subdirectory(graph)
add_subdirectory(misc)
