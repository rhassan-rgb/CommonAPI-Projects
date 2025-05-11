# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
# 
# Copyright (C) 2025 Ragab R. Hassan (r.elkattawy@gmail.com)

cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME "HelloEverything")
project(${PROJECT_NAME} CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17 -Wl,--no-as-needed")

find_package(CommonAPI 3.2.0 REQUIRED CONFIG NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_package(CommonAPI-SomeIP 3.2.0 REQUIRED CONFIG NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_package(vsomeip3 3.4.0 REQUIRED)

include_directories(
    src-gen/core/common
    src-gen/core/skel
    src-gen/core/proxy
    src-gen/core/stub
    src-gen/someip/common
    src-gen/someip/proxy
    src-gen/someip/stub
    src
    ${COMMONAPI_INCLUDE_DIRS}
    ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
    ${VSOMEIP_INCLUDE_DIRS}
)

link_directories(
    ${CMAKE_SOURCE_DIR}/../install_folder/lib
)

set(COMMON_LIB ${PROJECT_NAME}-common)
FILE(GLOB_RECURSE COMMON_LIB_SRCS src-gen/someip/common/*.cpp)
add_library(${COMMON_LIB} SHARED ${COMMON_LIB_SRCS})
target_link_libraries(${COMMON_LIB} CommonAPI-SomeIP)

set(PROXY_LIB ${PROJECT_NAME}-proxy)
FILE(GLOB_RECURSE PROXY_LIB_SRCS src-gen/someip/proxy/*.cpp)
add_library(${PROXY_LIB} SHARED ${PROXY_LIB_SRCS})
target_link_libraries(${PROXY_LIB} CommonAPI-SomeIP)

set(STUB_LIB ${PROJECT_NAME}-stub)
FILE(GLOB_RECURSE STUB_LIB_SRCS src-gen/someip/stub/*.cpp)
add_library(${STUB_LIB} SHARED ${STUB_LIB_SRCS})
target_link_libraries(${STUB_LIB} CommonAPI-SomeIP)

