cmake_minimum_required(VERSION 3.14)

project(PBRMaterialEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/materialeditor.h
    src/materialeditor.cpp
    src/materialpreview.h
    src/materialpreview.cpp
    src/resources.qrc
)

qt_add_executable(PBRMaterialEditor
    ${PROJECT_SOURCES}
)

target_link_libraries(PBRMaterialEditor PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGL)
