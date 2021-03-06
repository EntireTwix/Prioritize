cmake_minimum_required(VERSION 3.10)
project(priority)
set (CMAKE_CXX_STANDARD 17)

file(GLOB SRC "src/imgui/*.cpp")
add_executable(${PROJECT_NAME} ${SRC} src/imgui/backends/imgui_impl_opengl2.cpp src/imgui/backends/imgui_impl_glut.cpp main.cpp)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (MSVC)	
    include_directories(${FREEGLUT_INCLUDE_DIRS})
    set(LIBS ${FREEGLUT_LIBRARY} opengl32)
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} /usr/local/lib/libglut.so)
    target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS} /usr/local/include)
else() 
    set(LIBS glut GL GLU)
    target_link_libraries(${PROJECT_NAME} ${LIBS})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC src/imgui)
target_include_directories(${PROJECT_NAME} PUBLIC src/json/single_include)
