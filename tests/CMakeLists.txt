set(TEST_NAMES
    test_boolean
    test_integer
    test_floating
    test_complex
    test_string
    test_array
    test_vector
    test_deque
    test_list
    test_forward_list
    test_valarray
    test_bitset
    test_set
    test_unordered_set
    test_map
    test_unordered_map
    test_pair
    test_tuple
    test_random
    test_tag
    test_in_place
    test_user_defined
    test_inheritance
    )

if("${CMAKE_CXX_STANDARD}" STREQUAL "17")
    set(TEST_NAMES ${TEST_NAMES} test_optional)
endif()

foreach(TEST_NAME ${TEST_NAMES})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)

    if(WAD_TEST_COVERAGE)
        target_compile_options(${TEST_NAME} PRIVATE "-Og" PRIVATE "-g" PRIVATE "--coverage")
        target_link_options(${TEST_NAME} PRIVATE "--coverage")
    else()
        target_compile_options(${TEST_NAME} PRIVATE "-O2")
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME}
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endforeach(TEST_NAME)
