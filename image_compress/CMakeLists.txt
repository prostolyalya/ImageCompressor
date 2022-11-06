cmake_minimum_required(VERSION 3.5)

project(LibConvert LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)
aux_source_directory(source/ sources)
set(headers
    include/ImageCompressor.h
    include/Image.h
    include/Bitmap.h
)

add_library(ImageCompressLib STATIC
    ${sources}
    ${headers}
)
