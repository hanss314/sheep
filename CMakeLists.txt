cmake_minimum_required(VERSION 3.12)
project(Sheep C)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(CMAKE_C_STANDARD 11)

bison_target(parser parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
flex_target(lexer parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
add_flex_bison_dependency(lexer parser)

add_library(language STATIC
        ${sources}
        "${CMAKE_CURRENT_BINARY_DIR}/parser.c"
        "${CMAKE_CURRENT_BINARY_DIR}/lexer.c"
)

add_executable(sheep main.c)
target_link_libraries(sheep language)
add_dependencies(sheep language)
target_include_directories(language PRIVATE include)
target_include_directories(sheep PRIVATE include)
add_compile_options(-fsanitize=address,undefined,leak)
target_link_libraries(sheep asan ubsan)

