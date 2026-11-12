cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME "MediaControl")
project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(CommonAPI 3.2.0 REQUIRED CONFIG)
find_package(CommonAPI-SomeIP 3.2.0 REQUIRED)
find_package(vsomeip3 3.7.0 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(DLT REQUIRED IMPORTED_TARGET automotive-dlt)
# On macOS/Darwin, librt does not exist (POSIX realtime functions are in libSystem)
if(APPLE AND TARGET PkgConfig::DLT)
    get_target_property(_dlt_link_libs PkgConfig::DLT INTERFACE_LINK_LIBRARIES)
    if(_dlt_link_libs)
        list(REMOVE_ITEM _dlt_link_libs "rt")
        set_target_properties(PkgConfig::DLT PROPERTIES INTERFACE_LINK_LIBRARIES "${_dlt_link_libs}")
    endif()
endif()

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
FetchContent_MakeAvailable(googletest)
enable_testing()

link_directories(
    ${COMMONAPI_LIBDIR}
)

set(SOMEIP_COMMON_LIB ${PROJECT_NAME}someipcommon)
file(GLOB_RECURSE SOMEIP_COMMON_LIB_SRCS src-gen/someip/common/*.cpp)
add_library(${SOMEIP_COMMON_LIB} SHARED)
target_sources(${SOMEIP_COMMON_LIB} PRIVATE ${SOMEIP_COMMON_LIB_SRCS})
target_include_directories(${SOMEIP_COMMON_LIB} PUBLIC 
    src-gen/core/common
    src-gen/someip/common
    ${COMMONAPI_INCLUDE_DIRS}
    ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
    ${VSOMEIP_INCLUDE_DIRS}
)
target_link_libraries(${SOMEIP_COMMON_LIB} PUBLIC CommonAPI-SomeIP)
#=============================================================
set(SOMEIP_PROXY_LIB ${PROJECT_NAME}someipproxy)
file(GLOB_RECURSE SOMEIP_PROXY_LIB_SRCS src-gen/someip/proxy/*.cpp)
add_library(${SOMEIP_PROXY_LIB} SHARED)
target_sources(${SOMEIP_PROXY_LIB} PRIVATE ${SOMEIP_PROXY_LIB_SRCS})
target_include_directories(${SOMEIP_PROXY_LIB} PUBLIC
    src-gen/core/proxy
    src-gen/someip/proxy
)
target_link_libraries(${SOMEIP_PROXY_LIB} PUBLIC CommonAPI-SomeIP ${SOMEIP_COMMON_LIB})

#=============================================================
set(SOMEIP_STUB_LIB ${PROJECT_NAME}someipstub)
file(GLOB_RECURSE SSOMEIP_STUB_LIB_SRCS src-gen/someip/stub/*.cpp)
add_library(${SOMEIP_STUB_LIB} SHARED)
target_sources(${SOMEIP_STUB_LIB} PRIVATE ${SSOMEIP_STUB_LIB_SRCS})
target_include_directories(${SOMEIP_STUB_LIB} PUBLIC
    src-gen/core/stub
    src-gen/core/skel
    src-gen/someip/stub
)
target_link_libraries(${SOMEIP_STUB_LIB} PUBLIC CommonAPI-SomeIP ${SOMEIP_COMMON_LIB})


add_subdirectory(src/common)
add_subdirectory(src/client)
add_subdirectory(src/server)
add_subdirectory(test)

set_target_properties(${PROJECT_NAME}Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(${PROJECT_NAME}Service PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})