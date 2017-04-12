# prevent custom command generate clean
# https://cmake.org/cmake/help/v2.8.8/cmake.html#prop_dir%3aCLEAN_NO_CUSTOM
set_property(DIRECTORY APPEND PROPERTY CLEAN_NO_CUSTOM 1)
#set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES phxrpc_*)

set(PB_SRC storesvr.pb.cc storesvr.pb.h)
add_custom_command(
        OUTPUT ${PB_SRC}
        COMMAND protoc -I${BASERPC_INCLUDE_DIRS} --cpp_out=. -I. storesvr.proto
)


set(PB_SRC2 sectioninfo.pb.cc sectioninfo.pb.h)
add_custom_command(
        OUTPUT ${PB_SRC2}
        COMMAND protoc -I${BASERPC_INCLUDE_DIRS} --cpp_out=. -I. sectioninfo.proto
)

set(CLI_SRCS storesvr_client.cpp phxrpc_storesvr_stub.cpp)
add_custom_command(
        OUTPUT ${CLI_SRCS}
        COMMAND phxrpc_pb2client -I ${BASERPC_INCLUDE_DIRS} -f storesvr.proto -d .
)

set(SVR_SRCS phxrpc_storesvr_service.cpp storesvr_service_impl.cpp phxrpc_storesvr_dispatcher.cpp storesvr_server_config.cpp storesvr_main.cpp)
add_custom_command(
        OUTPUT ${SVR_SRCS}ba
        COMMAND phxrpc_pb2service -I ${BASERPC_INCLUDE_DIRS} -f storesvr.proto -d .
        COMMAND phxrpc_pb2server -I ${BASERPC_INCLUDE_DIRS} -f storesvr.proto -d .
)

set(TOOL_SRCS phxrpc_storesvr_tool.cpp storesvr_tool_impl.cpp storesvr_tool_main.cpp)
add_custom_command(
        OUTPUT ${TOOL_SRCS}
        COMMAND phxrpc_pb2tool -I ${BASERPC_INCLUDE_DIRS} -f storesvr.proto -d .
)

#add_library(storesvr_service ${SVC_SRCS} ${PB_SRC}) storesvr_service
add_library(storesvr_test ${SVR_SRCS} ${PB_SRC} ${PB_SRC2})
add_executable(storesvr_main ${SVR_SRCS} ${PB_SRC}  ${PB_SRC2} store_bin_util.cpp store_bin_util.h  backward.cpp)
target_link_libraries(storesvr_main routerinfo protobuf pthread phxrpc tcmalloc_and_profiler)

add_library(storesvr_client ${CLI_SRCS} ${PB_SRC}  ${PB_SRC2})
add_executable(storesvr_tool_main ${TOOL_SRCS})
target_link_libraries(storesvr_tool_main storesvr_client routerinfo protobuf pthread phxrpc)