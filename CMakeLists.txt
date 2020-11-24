cmake_minimum_required (VERSION 3.1)
project (TreeVisualizer)

set(CMAKE_CXX_STANDARD 14)

#to setup sdl,sdl_ttf; chek cmake_modules dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(OpenGL REQUIRED)
# look for glew relative to the "GLEW" environment variable path
# Need to set GLEW_INCLUDE_DIR and GLEW_LIBRARY_RELEASE
IF(NOT(APPLE))
  set(GLEW_INCLUDE_DIR $ENV{GLEW}/include)
  set(GLEW_LIBRARY_RELEASE $ENV{GLEW}/lib/Release/Win32/glew32.lib)
  find_package(GLEW REQUIRED)
ENDIF()

#directories to include 
set(INCLUDE_DIRECTORIES
  ${SDL2_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/TreeVisualizer/vendors/imgui)

  #libraries to link
set(LINK_LIBRARIES ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})

IF(NOT(APPLE))
	list(APPEND INCLUDE_DIRECTORIES ${GLEW_INCLUDE_DIRS})
  list(APPEND LINK_LIBRARIES ${GLEW_LIBRARIES})
ENDIF()

include_directories(${INCLUDE_DIRECTORIES})

# we should put our source file which we want to build manually
set (SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/TreeVisualizer/main.cpp
    ${CMAKE_SOURCE_DIR}/TreeVisualizer/vendors/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/TreeVisualizer/vendors/imgui/imgui_impl_sdl.cpp
    ${CMAKE_SOURCE_DIR}/TreeVisualizer/vendors/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/TreeVisualizer/vendors/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/TreeVisualizer/vendors/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/TreeVisualizer/vendors/imgui/imgui_impl_opengl3.cpp
   )

add_executable(TreeVisualizer ${SOURCE_FILES})
target_link_libraries(TreeVisualizer ${LINK_LIBRARIES})




