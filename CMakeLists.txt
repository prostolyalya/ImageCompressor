cmake_minimum_required(VERSION 3.14)

project(image_compressor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        source/controller.cpp
        source/ui_controller.cpp
        source/file_model.cpp
)

set(PROJECT_HEADERS
    include/controller.h
    include/ui_controller.h
    include/file_model.h
)

add_subdirectory(image_compress)
add_executable(image_compressor
  ${PROJECT_SOURCES}
  ${PROJECT_HEADERS}
)
target_compile_definitions(image_compressor
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(image_compressor
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick ImageCompressLib)

