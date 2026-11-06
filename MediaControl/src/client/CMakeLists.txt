
add_library(clientLib STATIC)

target_sources(clientLib PRIVATE
    secoc/SecOcClient.cpp

    someIp/RemoteMediaProxyImpl.cpp

    client/MediaControlClient.cpp
)

target_include_directories(clientLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(clientLib PUBLIC common CommonAPI ${SOMEIP_PROXY_LIB} ${SOMEIP_COMMON_LIB} CommonAPI-SomeIP vsomeip3)

add_executable(${PROJECT_NAME}Client main.cpp)
target_link_libraries(${PROJECT_NAME}Client PRIVATE clientLib)