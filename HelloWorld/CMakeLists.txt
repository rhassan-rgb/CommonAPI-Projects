# cmake_minimum_required(VERSION 2.8)

# set(PROJ_NAME HelloWorld)
# project(${PROJ_NAME} CXX)
# set(CMAKE_VERBOSE_MAKEFILE on)

# OPTION(USE_FILE "Set to OFF to disable file logginf" OFF)
# message(STATUS "USE_FILE is set to value: ${USE_FILE}")

# OPTION(USE_CONSOLE "Set to OFF to disable console logging" OFF)
# message(STATUS "USE_CONSOLE is set to value: ${USE_CONSOLE}")

# IF(USE_FILE)
#     add_definitions(-DUSE_FILE)
# ENDIF(USE_FILE)

# IF(USE_CONSOLE)
#     add_definitions(-DUSE_CONSOLE)
# ENDIF(USE_CONSOLE)

# set(MAX_LOG_LEVEL "Debug" CACHE STRING "maximum log level")
# message(STATUS "MAX_LOG_LEVEL is set to value: ${MAX_LOG_LEVEL}")
# add_definitions(-DCOMMONAPI_LOGLEVEL=COMMONAPI_LOGLEVEL_${MAX_LOG_LEVEL})

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -O0 -std=c++11 -D_GLIBCXX_USE_NANOSLEEP -DLINUX")

# message(STATUS "Compiler Options:  ${CMAKE_CXX_FLAGS}")

# if (NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release." FORCE)
# endif(NOT CMAKE_BUILD_TYPE)
# message (STATUS "Build type: ${CMAKE_BUILD_TYPE}")
# # You can have an uninstalled version of CommonAPI, it will affect the find_package()
# OPTION(USE_INSTALLED_COMMONAPI "Set to OFF to use the local version of CommonAPI" ON)
# message (STATUS "USE_INSTALLED_COMMONAPI is set to value: ${USE_INSTALLED_COMMONAPI}")

# if("${USE_INSTALLED_COMMONAPI}" STREQUAL "ON")
#     find_package(CommonAPI 3.2.0 REQUIRED CONFIG NO_CMAKE_PACKAGE_REGISTRY)
# else()
#     find_package(CommonAPI 3.2.0 REQUIRED CONFIG NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
# endif()

# message(STATUS "CommonAPI_CONSIDERED_CONFIGS: ${CommonAPI_CONSIDERED_CONFIGS}")
# message(STATUS "COMMONAPI_INCLUDE_DIRS: ${COMMONAPI_INCLUDE_DIRS}")

# #CommonAPI

# # We already have CMAKE_PREFIX_PATH defined
# #SOME/IP
# find_package (CommonAPI-SomeIP 3.2.0 REQUIRED CONFIG NO_CMAKE_PACKAGE_REGISTRY)
# find_package (vsomeip3 3.1.0 REQUIRED)

# # Source Files
# set(PRJ_SRC_PATH src)

# # Generated sources location
# set(PRJ_SRC_GEN_PATH src-gen)

# #Generated CommonAPI core files
# set(PRJ_SRC_GEN_COMMONAPI_PATH ${PRJ_SRC_GEN_PATH}/core/v0/commonapi/examples)

# #generated CommonAPI-SomeIp runtime files
# set(PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH ${PRJ_SRC_GEN_PATH}/someip/v0/commonapi/examples)

# # Set Executables Names
# set(PROJ_NAME_CLIENT ${PROJ_NAME}Client)
# set(PROJ_NAME_SERVICE ${PROJ_NAME}Service)

# # Collect the sources
# # Proxy Sources
# FILE(GLOB PRJ_PROXY_GEN_SRCS ${PRJ_SRC_GEN_COMMONAPI_PATH}/*Proxy.cpp)
# # Stub Sources
# FILE(GLOB PRJ_STUB_GEN_SRCS ${PRJ_SRC_GEN_COMMONAPI_PATH}/*Stub*.cpp)
# # 
# FILE(GLOB PRJ_STUB_IMPL_SRCS ${PRJ_SRC_PATH}/stub-impl/*.cpp)

# set(PRJ_CLIENT_SRCS ${PRJ_SRC_PATH}/${PROJ_NAME_CLIENT}.cpp ${PRJ_PROXY_GEN_SRCS})
# set(PRJ_SERVICE_SRCS ${PRJ_SRC_PATH}/${PROJ_NAME_SERVICE}.cpp ${PRJ_STUB_IMPL_SRCS} ${PRJ_STUB_GEN_SRCS} ${PRJ_STUB_IMPL_SRCS})

# # Boost
# find_package( Boost 1.54 COMPONENTS system thread log REQUIRED )
# include_directories( ${Boost_INCLUDE_DIR} )

# # SOME/IP Interface Generated library
# FILE(GLOB PRJ_SOMEIP_LIB_SRCS ${PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH}/*cpp)

# include_directories(
#     src-gen/core
#     src-gen/dbus
#     src-gen/someip
#     src/stub-impl
#     ${COMMONAPI_INCLUDE_DIRS}
#     ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
#     ${VSOMEIP_INCLUDE_DIRS}
# )
# # install_folder/lib
# link_directories(
#     ${COMMONAPI_LIBDIR}
#     ${COMMONAPI_SOMEIP_CMAKE_DIR}/build
#     ${Boost_LIBRARY_DIR}
# )

# set(LINK_LIBRARIES -Wl,--as-needed CommonAPI)

# # Build Client
# add_executable(${PROJ_NAME_CLIENT} ${PRJ_CLIENT_SRCS})
# target_link_libraries(${PROJ_NAME_CLIENT} ${LINK_LIBRARIES})

# # Build service
# add_executable(${PROJ_NAME_SERVICE} ${PRJ_SERVICE_SRCS})
# target_link_libraries(${PROJ_NAME_SERVICE} ${LINK_LIBRARIES})

# # Build SOME/IP interface binding library
# add_library (${PROJ_NAME}-someip SHARED ${PRJ_SOMEIP_LIB_SRCS})
# target_link_libraries(${PROJ_NAME}-someip CommonAPI-SomeIP)


# cmake_minimum_required(VERSION 2.8)

# set(PRJ_NAME HelloWorld)

# set(CMAKE_VERBOSE_MAKEFILE on)

# OPTION(USE_FILE "Set to OFF to disable file logging" OFF )
# message(STATUS "USE_FILE is set to value: ${USE_FILE}")

# OPTION(USE_CONSOLE "Set to OFF to disable console logging" OFF )
# message(STATUS "USE_CONSOLE is set to value: ${USE_CONSOLE}")

# IF(USE_FILE)
#   add_definitions(-DUSE_FILE)
# ENDIF(USE_FILE)
# IF(USE_CONSOLE)
#   add_definitions(-DUSE_CONSOLE)
# ENDIF(USE_CONSOLE)

# SET(MAX_LOG_LEVEL "DEBUG" CACHE STRING "maximum log level")
# message(STATUS "MAX_LOG_LEVEL is set to value: ${MAX_LOG_LEVEL}")
# add_definitions(-DCOMMONAPI_LOGLEVEL=COMMONAPI_LOGLEVEL_${MAX_LOG_LEVEL})

# if (MSVC)
# # Visual C++ is not always sure whether he is really C++
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS /EHsc /wd\\\"4503\\\"")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /wd\\\"4503\\\"")
# else()
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -O0 -std=c++11 -D_GLIBCXX_USE_NANOSLEEP -DLINUX")
# endif()

# message(STATUS "Compiler options: ${CMAKE_CXX_FLAGS}") 

# if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
#        "Choose the type of build, options are: Debug Release." FORCE)
# endif(NOT CMAKE_BUILD_TYPE)
# message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# OPTION(USE_INSTALLED_COMMONAPI "Set to OFF to use the local (build tree) version of CommonAPI" ON)
# message(STATUS "USE_INSTALLED_COMMONAPI is set to value: ${USE_INSTALLED_COMMONAPI}")

# if ("${USE_INSTALLED_COMMONAPI}" STREQUAL "ON")
#     FIND_PACKAGE(CommonAPI 3.2.0 REQUIRED CONFIG NO_CMAKE_PACKAGE_REGISTRY)
# else()
#     FIND_PACKAGE(CommonAPI 3.2.0 REQUIRED CONFIG NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
# endif()

# message(STATUS "CommonAPI_CONSIDERED_CONFIGS: ${CommonAPI_CONSIDERED_CONFIGS}")
# message(STATUS "COMMONAPI_INCLUDE_DIRS: ${COMMONAPI_INCLUDE_DIRS}")

# # CommonAPI
# include(FindPkgConfig)
# ###############################################################################
# # find DBus by using the 'pkg-config' tool

# # SOME/IP
# find_package (CommonAPI-SomeIP 3.2.0 REQUIRED)
# find_package (vsomeip3 3.1.0 REQUIRED)

# # Source Files
# set(PRJ_SRC_PATH src)
# set(PRJ_SRC_GEN_PATH src-gen)
# set(PRJ_SRC_GEN_COMMONAPI_PATH ${PRJ_SRC_GEN_PATH}/core/v0/commonapi/examples)
# set(PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH ${PRJ_SRC_GEN_PATH}/someip/v0/commonapi/examples)

# set(PRJ_NAME_CLIENT ${PRJ_NAME}Client)
# set(PRJ_NAME_SERVICE ${PRJ_NAME}Service)

# # Application
# FILE(GLOB PRJ_PROXY_GEN_SRCS ${PRJ_SRC_GEN_COMMONAPI_PATH}/*Proxy.cpp)
# FILE(GLOB PRJ_STUB_GEN_SRCS ${PRJ_SRC_GEN_COMMONAPI_PATH}/*Stub*.cpp)
# FILE(GLOB PRJ_STUB_IMPL_SRCS ${PRJ_SRC_COMMONAPI_PATH}/*Stub*.cpp)
# set(PRJ_CLIENT_SRCS ${PRJ_SRC_PATH}/${PRJ_NAME_CLIENT}.cpp ${PRJ_PROXY_GEN_SRCS})
# set(PRJ_SERVICE_SRCS ${PRJ_SRC_PATH}/${PRJ_NAME_SERVICE}.cpp ${PRJ_SRC_PATH}/${PRJ_NAME}StubImpl.cpp ${PRJ_STUB_GEN_SRCS} ${PRJ_STUB_IMPL_SRCS})
# message ("PRJ_STUB_IMPL_SRCS:") 
# message("${PRJ_STUB_IMPL_SRCS}")
# message ("PRJ_STUB_IMPL_SRCS:") 

# # Boost
# find_package( Boost 1.54 COMPONENTS system thread log REQUIRED )
# include_directories( ${Boost_INCLUDE_DIR} )


# # SOME/IP library
# FILE(GLOB PRJ_SOMEIP_LIB_SRCS ${PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH}/*cpp)

# # Paths

# include_directories(
#     src-gen/core
#     src-gen/someip
#     ${COMMONAPI_INCLUDE_DIRS}
#     ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
#     ${VSOMEIP_INCLUDE_DIRS}
# )

# if ("${USE_INSTALLED_DBUS}" STREQUAL "ON")
# link_directories(
#     ${COMMONAPI_LIBDIR}
#     ${COMMONAPI_SOMEIP_CMAKE_DIR}/build
#     ${Boost_LIBRARY_DIR}
# )
# else()
# link_directories(
#     ${COMMONAPI_LIBDIR}
#     ${COMMONAPI_SOMEIP_CMAKE_DIR}/build
#     ${Boost_LIBRARY_DIR}
# )
# endif()

# if (MSVC)
#   set(LINK_LIBRARIES CommonAPI)
# else()
#   set(LINK_LIBRARIES -Wl,--as-needed CommonAPI)
# endif()

# # Build Client
# add_executable(${PRJ_NAME_CLIENT} ${PRJ_CLIENT_SRCS})
# target_link_libraries(${PRJ_NAME_CLIENT} ${LINK_LIBRARIES})

# # Build service
# add_executable(${PRJ_NAME_SERVICE} ${PRJ_SERVICE_SRCS})
# target_link_libraries(${PRJ_NAME_SERVICE} ${LINK_LIBRARIES})

# # Build D
# # Build SOME/IP library
# add_library (${PRJ_NAME}-someip SHARED ${PRJ_SOMEIP_LIB_SRCS})
# target_link_libraries(${PRJ_NAME}-someip CommonAPI-SomeIP)


cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME "HelloWorld")
project(${PROJECT_NAME} CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x -Wl,--no-as-needed")

# include(FindPkgConfig)
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