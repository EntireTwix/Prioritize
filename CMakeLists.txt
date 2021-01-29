cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-static-libstdc++")
project(priority)
add_executable(${PROJECT_NAME} main.cpp)

set(LIBS glut GL GLU)
target_link_libraries(${PROJECT_NAME} ${LIBS})


target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC third_party/imgui)
target_include_directories(${PROJECT_NAME} PUBLIC third_party/json/include)

target_sources(${PROJECT_NAME} 
                               PRIVATE third_party/imgui/imgui.cpp 
                               PRIVATE third_party/imgui/imgui_draw.cpp 
                               PRIVATE third_party/imgui/imgui_tables.cpp 
                               PRIVATE third_party/imgui/imgui_widgets.cpp 
                               PRIVATE third_party/imgui/imgui_demo.cpp
                               PRIVATE third_party/imgui/backends/imgui_impl_glut.cpp
                               PRIVATE third_party/imgui/backends/imgui_impl_opengl2.cpp
                ) 