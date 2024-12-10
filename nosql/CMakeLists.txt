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


add_executable(nosql main.cpp ${SOURCE_FILES})

