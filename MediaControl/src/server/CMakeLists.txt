
add_library(serverLib STATIC)

target_sources(serverLib PRIVATE
    secoc/SecOcServer.cpp

    someIp/RemoteMediaStubImpl.cpp

    service/MediaControlService.cpp
)

target_include_directories(serverLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(serverLib PUBLIC common CommonAPI ${SOMEIP_STUB_LIB} ${SOMEIP_COMMON_LIB} CommonAPI-SomeIP vsomeip3)

add_executable(${PROJECT_NAME}Service main.cpp)
target_link_libraries(${PROJECT_NAME}Service PRIVATE serverLib)