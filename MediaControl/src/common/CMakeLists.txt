
add_library(common STATIC)

target_sources(common PRIVATE
    secoc/ISecOc.cpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(common PUBLIC OpenSSL::Crypto)