cmake_minimum_required(VERSION 3.12)

project(imnodflow-example VERSION 0.1)
message("Project name: " ${CMAKE_PROJECT_NAME})

# Require dot, treat the other components as optional
find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia)

set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN NO)
set(DOXYGEN_GENERATE_LATEX YES)
set(DOXYGEN_USE_PDFLATEX YES)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ./ReadMe.md)
set(DOXYGEN_GENERATE_HTMLHELP YES)
set(DOXYGEN_EXTRACT_PRIVATE YES)
set(DOXYGEN_EXCLUDE_PATTERNS

)

doxygen_add_docs(
    doxygen
    ./ReadMe.md
    ${PROJECT_SOURCE_DIR}
    COMMENT "Generate html pages"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(OpenGL REQUIRED)
    add_library(glad ../glad/src/glad.c)
    find_package(glfw3 3.3 REQUIRED)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_package(OpenGL REQUIRED)
    add_library(glad ../glad/src/glad.c)
    find_library(GLFW glfw3 PATHS ../glfw/lib)
endif()

add_library(ImNodeFlow ../ImNodeFlow/src/ImNodeFlow.cpp)
add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)


include_directories(
    Nodes
    ../ImNodeFlow/include
    ../glad/include
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    ../glfw/include
endif()
    ../imgui
    ../imgui/backends
    ../imfilebrowser
    ../imgui/misc/cpp
    ../implot
)

set(SOURCES
    ${SOURCES}
    main.cpp
    Nodes/NodePlot2Values.cpp
    Nodes/NodeSetFloat.cpp
    Nodes/NodeSetInt.cpp
    ../imgui/imgui.cpp
    ../imgui/imgui_demo.cpp
    ../imgui/imgui_draw.cpp
    ../imgui/imgui_tables.cpp
    ../imgui/imgui_widgets.cpp
    ../imgui/backends/imgui_impl_opengl3.cpp
    ../imgui/backends/imgui_impl_glfw.cpp
    ../imgui/misc/cpp/imgui_stdlib.cpp
    ../implot/implot.cpp
    ../implot/implot_items.cpp
)

set(HEADERS
    ${HEADERS}
    Nodes/NodePlot2Values.hpp
    Nodes/NodeSetFloat.hpp
    Nodes/NodeSetInt.hpp
    ../imgui-common/AppImGuiCRTP.hpp
    ../imgui/imconfig.h
    ../imgui/imgui.h
    ../imgui/imgui_internal.h
    ../imgui/backends/imgui_impl_opengl3.h
    ../imgui/backends/imgui_impl_glfw.h
    ../imgui/misc/cpp/imgui_stdlib.h
    #../imfilebrowser/imfilebrowser.h
    ../implot/implot.h
    ../implot/implot_internal.h
)

add_compile_definitions(USE_IMPLOT)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

# remove dosbox with linker option "-mwindows"
# target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -mwindows -static-libgcc -static-libstdc++)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
else()
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -mwindows -static-libgcc -static-libstdc++)
endif()
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE APP_NAME="${CMAKE_PROJECT_NAME}")
target_link_libraries(${CMAKE_PROJECT_NAME}
    -static
    ${OPENGL_gl_LIBRARY}
    glad
    ${GLFW}
    ImNodeFlow
    winpthread
    -dynamic
    #ws2_32
)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE APP_NAME="${CMAKE_PROJECT_NAME}")
# Added "-no-pie" linker option so that this can also be called from "nautilus".
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -no-pie)
target_link_libraries(${CMAKE_PROJECT_NAME}
    glfw
    OpenGL::GL
    glad
    ImNodeFlow
    pthread
    ${CMAKE_DL_LIBS}
)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  find_library(COCOA_LIBRARY Cocoa)
  find_library(OPENGL_LIBRARY OpenGL)
  find_library(IOKIT_LIBRARY IOKit)
  find_library(COREVIDEO_LIBRARY CoreVideo)
  set(EXTRA_LIBS
    ${COCOA_LIBRARY}
    ${OPENGL_LIBRARY}
    ${IOKIT_LIBRARY}
    ${COREVIDEO_LIBRARY}
    )
    target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ../glfw/lib-x86_64)
    target_link_libraries(${CMAKE_PROJECT_NAME} glfw3 ImNodeFlow ${EXTRA_LIBS})
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
      APP_NAME="${CMAKE_PROJECT_NAME}"    
      GL_SILENCE_DEPRECATION
    )
endif()

add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/${APP_NAME} DEPENDS ${APP_NAME})
