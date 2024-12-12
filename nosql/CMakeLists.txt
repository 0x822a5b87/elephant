cmake_minimum_required(VERSION 3.17)

project(nosql)

set(CMAKE_CXX_STANDARD 11)

set(HEADER_FILES
        src/data_access_layer.hpp
        src/freelist.hpp
        src/page.hpp
        src/meta.hpp
        src/item.hpp
        src/node.hpp
)

set(SOURCE_FILES
        src/data_access_layer.cpp
        src/freelist.cpp
        src/page.cpp
        src/meta.cpp
        src/item.cpp
        src/node.cpp
)

set(RAPIDJSON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/rapidjson/include)

include_directories(${RAPIDJSON_INCLUDE_DIRS})

add_executable(nosql src/main.cpp ${SOURCE_FILES})

