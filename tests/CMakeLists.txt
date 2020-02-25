set(TEST_NAMES
    test_boolean
    test_integer
    test_floating
    test_string
    test_array
    test_vector
    test_deque
    test_list
    test_forward_list
    test_set
    test_unordered_set
    test_map
    test_unordered_map
    )

foreach(TEST_NAME ${TEST_NAMES})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)

    set_target_properties(${TEST_NAME} PROPERTIES
        COMPILE_FLAGS "${MJOLNIR_WARNING_FLAGS} -O2")

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME}
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endforeach(TEST_NAME)
