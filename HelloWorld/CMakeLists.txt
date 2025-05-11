 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
# 
# Copyright (C) 2025 Ragab R. Hassan (r.elkattawy@gmail.com)
# 
# The source of this CMake file can be found here:
# https://github.com/COVESA/capicxx-core-tools/wiki/Loading-Bindings-And-Libraries 

cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME "HelloWorld")
project(${PROJECT_NAME} CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x -Wl,--no-as-needed")

find_package(CommonAPI 3.2.0 REQUIRED CONFIG NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_package(CommonAPI-SomeIP 3.2.0 REQUIRED)
find_package(vsomeip3 3.4.0 REQUIRED)

include_directories(
     src-gen/core/common
     src-gen/core/proxy
     src-gen/core/stub
     src-gen/core/skel
     src-gen/someip/common
     src-gen/someip/proxy
     src-gen/someip/stub
     ${COMMONAPI_INCLUDE_DIRS}
     ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
     ${VSOMEIP_INCLUDE_DIRS}
)

link_directories(
    ${COMMONAPI_LIBDIR}
    ${COMMONAPI_SOMEIP_CMAKE_DIR}/build
)

# FILE(GLOB_RECURSE HELLOWORLD_DBUS_COMMON_LIB_SRCS src-gen/dbus/common/*.cpp)
# add_library(helloworlddbuscommon SHARED ${HELLOWORLD_DBUS_COMMON_LIB_SRCS})
# target_link_libraries(helloworlddbuscommon CommonAPI-DBus dbus-1)

# FILE(GLOB_RECURSE HELLOWORLD_DBUS_PROXY_LIB_SRCS src-gen/dbus/proxy/*.cpp)
# add_library(helloworlddbusproxy SHARED ${HELLOWORLD_DBUS_PROXY_LIB_SRCS})
# target_link_libraries(helloworlddbusproxy CommonAPI-DBus dbus-1)

# FILE(GLOB_RECURSE HELLOWORLD_DBUS_STUB_LIB_SRCS src-gen/dbus/stub/*.cpp)
# add_library(helloworlddbusstub SHARED ${HELLOWORLD_DBUS_STUB_LIB_SRCS})
# target_link_libraries(helloworlddbusstub CommonAPI-DBus dbus-1)
set(SOMEIP_COMMON_LIB ${PROJECT_NAME}someipcommon)
FILE(GLOB_RECURSE HELLOWORLD_SOMEIP_COMMON_LIB_SRCS src-gen/someip/common/*.cpp)
add_library(${SOMEIP_COMMON_LIB} SHARED ${HELLOWORLD_SOMEIP_COMMON_LIB_SRCS})
target_link_libraries(${SOMEIP_COMMON_LIB} CommonAPI-SomeIP)

set(SOMEIP_PROXY_LIB ${PROJECT_NAME}someipproxy)
FILE(GLOB_RECURSE HELLOWORLD_SOMEIP_PROXY_LIB_SRCS src-gen/someip/proxy/*.cpp)
add_library(${SOMEIP_PROXY_LIB} SHARED ${HELLOWORLD_SOMEIP_PROXY_LIB_SRCS})
target_link_libraries(${SOMEIP_PROXY_LIB} CommonAPI-SomeIP)

set(SOMEIP_STUB_LIB ${PROJECT_NAME}someipstub)
FILE(GLOB_RECURSE HELLOWORLD_SOMEIP_STUB_LIB_SRCS src-gen/someip/stub/*.cpp)
add_library(${SOMEIP_STUB_LIB} SHARED ${HELLOWORLD_SOMEIP_STUB_LIB_SRCS})
target_link_libraries(${SOMEIP_STUB_LIB} CommonAPI-SomeIP)

add_executable(${PROJECT_NAME}Client src/HelloWorldClient.cpp)
target_link_libraries(${PROJECT_NAME}Client CommonAPI ${SOMEIP_PROXY_LIB} ${SOMEIP_COMMON_LIB} CommonAPI-SomeIP vsomeip3)

add_executable(${PROJECT_NAME}Service src/HelloWorldService.cpp src/HelloWorldStubImpl.cpp)
target_link_libraries(${PROJECT_NAME}Service CommonAPI ${SOMEIP_STUB_LIB} ${SOMEIP_COMMON_LIB} CommonAPI-SomeIP vsomeip3)


#VSOMEIP_CONFIGURATION=$(realpath ./helloworld-local.json) COMMONAPI_CONFIG=commonapi4someip.ini LD_LIBRARY_PATH=../../install_path/lib:$PWD/build/  build/HelloWorldClient