cmake_minimum_required(VERSION 3.10)
project(grass LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(deps/glfw-3.4)

find_package(OpenGL REQUIRED)

add_executable(grass 
    main.cpp 
    glad.c 
    shader.h 
    shader.cpp 
    mesh.h 
    mesh.cpp 
    skybox.h 
    skybox.cpp
    stb_image.cpp
)

target_include_directories(grass PRIVATE ${CMAKE_SOURCE_DIR}/deps/include)

target_link_libraries(grass PRIVATE
    glfw
	OpenGL::GL
)

add_custom_command(TARGET grass POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    $<TARGET_FILE_DIR:grass>/res
)