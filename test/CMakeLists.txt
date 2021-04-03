add_executable(parser parser.cpp)
target_link_libraries(parser PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(parser_test parser)

add_executable(token token.cpp)
target_link_libraries(token PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(token_test token)