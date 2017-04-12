enable_testing()

##############
# Unit Tests
##############
add_executable(commonTest rand_util.cpp comm_test_main.cpp routerinfo_util_test.cpp routerlist_diff_test.cpp macros.h backward.cpp)

# Standard linking to gtest stuff.
target_link_libraries(commonTest routerinfo protobuf pthread gtest gtest_main -pthread tcmalloc_and_profiler)
