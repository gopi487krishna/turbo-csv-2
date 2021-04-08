add_executable(parser parser.cpp)
target_link_libraries(parser PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(parser_test parser)

add_executable(event event.cpp)
target_link_libraries(event PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(event_test event)

add_executable(memstream memstream.cpp)
target_link_libraries(memstream PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(memstream_test memstream)
