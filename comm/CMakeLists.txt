# prevent custom command generate clean
# https://cmake.org/cmake/help/v2.8.8/cmake.html#prop_dir%3aCLEAN_NO_CUSTOM
set_property(DIRECTORY APPEND PROPERTY CLEAN_NO_CUSTOM 1)

set(PB_SRC routerinfo.pb.cc routerinfo.pb.h)
add_custom_command(
        OUTPUT ${PB_SRC}
        COMMAND protoc -I${BASERPC_INCLUDE_DIRS} --cpp_out=. -I. routerinfo.proto
)

add_library(routerinfo ${PB_SRC} router_list_diff.h router_list_diff.cpp routerinfo_util.cpp routerinfo_util.h)