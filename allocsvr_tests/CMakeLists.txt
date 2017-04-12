enable_testing()

##############
# Unit Tests
##############
add_executable(allocSvrTest alloc_client_test.cpp sect_seq_test.cpp allocsvr_test_main.cpp)

# Standard linking to gtest stuff.
target_link_libraries(allocSvrTest allocsvr_service protobuf pthread phxrpc gtest gtest_main -pthread tcmalloc_and_profiler)
#
### This is so you can do 'make test' to see all your tests run, instead of
### manually running the executable runUnitTests to see those specific tests.
#add_test(NAME that-test-I-made COMMAND runUnitTests)
##
### You can also omit NAME and COMMAND. The second argument could be some other
### test executable.
#add_test(that-other-test-I-made runUnitTests)