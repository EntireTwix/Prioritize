cmake_minimum_required(VERSION 3.10)
project(priority)
set (CMAKE_CXX_STANDARD 17)

set(GitHubAPI_CurlBackend ON)
add_subdirectory(src/github_api)

file(GLOB SRC "src/imgui/*.cpp")
add_executable(${PROJECT_NAME} ${SRC} src/imgui/backends/imgui_impl_opengl2.cpp src/imgui/backends/imgui_impl_glut.cpp main.cpp)

if (MSVC)	
    include_directories(${FREEGLUT_INCLUDE_DIRS})
    set(LIBS ${FREEGLUT_LIBRARY} opengl32 github_api)
else() 
    set(LIBS glut GL GLU github_api)
endif()

target_link_libraries(${PROJECT_NAME} ${LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC src/imgui)
target_include_directories(${PROJECT_NAME} PUBLIC src/json/single_include)
