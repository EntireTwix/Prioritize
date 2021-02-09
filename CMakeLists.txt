cmake_minimum_required(VERSION 3.10)
project(priority)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(GitHubAPI_CurlBackend ON)
add_subdirectory(src/github_api)

file(GLOB_RECURSE SRC "src/*.hpp")

add_executable(${PROJECT_NAME} main.cpp)

if (MSVC)
    #target_link_directories(sdrpp_core PUBLIC FREEGLUT_LIB_PATH)
    #target_include_directories(${PROJECT_NAME} PUBLIC FREEGLUT_INC_PATH)
    set(LIBS freeglut opengl32 github_api)
else()
    set(LIBS glut GL GLU github_api)
endif()

target_link_libraries(${PROJECT_NAME} ${LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC src)