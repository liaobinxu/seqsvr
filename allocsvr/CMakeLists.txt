# prevent custom command generate clean
# https://cmake.org/cmake/help/v2.8.8/cmake.html#prop_dir%3aCLEAN_NO_CUSTOM
set_property(DIRECTORY APPEND PROPERTY CLEAN_NO_CUSTOM 1)
#set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES phxrpc_*)

set(PB_SRC allocsvr.pb.cc allocsvr.pb.h)
add_custom_command(
        OUTPUT ${PB_SRC}
        COMMAND protoc -I${BASERPC_INCLUDE_DIRS} --cpp_out=. -I. allocsvr.proto
)

set(CLI_SRCS allocsvr_client.cpp phxrpc_allocsvr_stub.cpp)
add_custom_command(
        OUTPUT ${CLI_SRCS}
        COMMAND phxrpc_pb2client -I ${BASERPC_INCLUDE_DIRS} -f allocsvr.proto -d .
)

set(SVC_SRCS phxrpc_allocsvr_service.cpp allocsvr_service_impl.cpp phxrpc_allocsvr_dispatcher.cpp alloc_seq.cpp storesvr_proxy.cpp sect_seq.cpp)
add_custom_command(
        OUTPUT ${SVC_SRCS}
        COMMAND phxrpc_pb2service -I ${BASERPC_INCLUDE_DIRS} -f allocsvr.proto -d .
)

set(SVR_SRCS allocsvr_server_config.cpp allocsvr_main.cpp)
add_custom_command(
        OUTPUT ${SVR_SRCS}
        COMMAND phxrpc_pb2server -I ${BASERPC_INCLUDE_DIRS} -f allocsvr.proto -d .
)

set(TOOL_SRCS phxrpc_allocsvr_tool.cpp allocsvr_tool_impl.cpp allocsvr_tool_main.cpp)
add_custom_command(
        OUTPUT ${TOOL_SRCS}
        COMMAND phxrpc_pb2tool -I ${BASERPC_INCLUDE_DIRS} -f allocsvr.proto -d .
)

add_library(allocsvr_client ${CLI_SRCS} ${PB_SRC})
add_library(allocsvr_service ${SVC_SRCS} ${PB_SRC})

add_executable(allocsvr_main ${PB_SRC} ${SVR_SRCS})
target_link_libraries(allocsvr_main allocsvr_service routerinfo protobuf pthread phxrpc tcmalloc_and_profiler)

add_executable(allocsvr_tool_main ${TOOL_SRCS})
target_link_libraries(allocsvr_tool_main allocsvr_client routerinfo protobuf pthread phxrpc)