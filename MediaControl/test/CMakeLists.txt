include(GoogleTest)

add_executable(test_common)

target_sources(test_common PRIVATE 
    common/test_secoc.cpp
)

target_link_libraries(test_common PRIVATE common GTest::gtest_main)
gtest_discover_tests(test_common)


add_executable(test_clientLib)

target_sources(test_clientLib PRIVATE
    client/test_secoc.cpp
    client/test_MediaRemoteClientImpl.cpp
)

target_link_libraries(test_clientLib PRIVATE clientLib GTest::gtest_main GTest::gmock)
gtest_discover_tests(test_clientLib)

add_executable(test_serverLib)

target_sources(test_serverLib PRIVATE
    server/test_secoc.cpp
    server/test_MediaRemoteServiceImpl.cpp
)

target_link_libraries(test_serverLib PRIVATE serverLib GTest::gtest_main GTest::gmock)
gtest_discover_tests(test_serverLib)