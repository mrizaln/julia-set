cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

project("Mandelbrot Set" VERSION 1.0 LANGUAGES C)

add_subdirectory(lib/glad)

# external library
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.c)

add_executable(main main.c ${SRC_FILES})

target_include_directories(main PUBLIC include)

target_compile_options(main PRIVATE
    -fsanitize=address
    -fsanitize=undefined
    -fsanitize=leak
)
target_link_options(main PRIVATE
    -fsanitize=address
    -fsanitize=undefined
    -fsanitize=leak
)

target_link_libraries(main PUBLIC m glfw glad::glad)

# copy resources to build directory
add_custom_command(TARGET main POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E create_symlink
                       ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:main>/resources)
