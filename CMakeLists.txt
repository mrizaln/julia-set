cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

project("Mandelbrot Set" VERSION 1.0)

add_subdirectory(lib/glad)

# external library
find_package(glfw3 3.3 REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.c)

add_executable(main main.c ${SRC_FILES})

target_include_directories(main PUBLIC include)

target_link_libraries(main PUBLIC glfw)
target_link_libraries(main PUBLIC glad)
target_link_libraries(main PUBLIC m)    # libm <math.h> library

# copy resources to build directory
add_custom_command(TARGET main POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:main>/resources)
