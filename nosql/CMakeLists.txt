cmake_minimum_required(VERSION 3.17)

project(nosql)

set(CMAKE_CXX_STANDARD 11)

set(HEADER_FILES
        data_access_layer.hpp
        freelist.hpp
        page.hpp
        meta.hpp
)

set(SOURCE_FILES
        data_access_layer.cpp
        freelist.cpp
        page.cpp
        meta.cpp
)

set(RAPIDJSON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/rapidjson/include)

include_directories(${RAPIDJSON_INCLUDE_DIRS})

add_executable(nosql main.cpp ${SOURCE_FILES})

