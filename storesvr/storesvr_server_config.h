/* storesvr_server_config.h

 Generated by phxrpc_pb2server from storesvr.proto

*/

#pragma once

#include "phxrpc/rpc.h"

class StoresvrServerConfig
{
public:
    StoresvrServerConfig();

    ~StoresvrServerConfig();

    bool Read( const char * config_file );

    phxrpc::HshaServerConfig & GetHshaServerConfig();

private:
    phxrpc::HshaServerConfig ep_server_config_;
};
