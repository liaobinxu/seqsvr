/* phxrpc_allocsvr_service.cpp

 Generated by phxrpc_pb2service from allocsvr.proto

 Please DO NOT edit unless you know exactly what you are doing.

*/

#include "phxrpc_allocsvr_service.h"
#include "allocsvr.pb.h"
#include "phxrpc/file.h"

AllocsvrService :: AllocsvrService()
{
}

AllocsvrService :: ~AllocsvrService()
{
}

int AllocsvrService :: PHXEcho( const google::protobuf::StringValue & /* req */,
        google::protobuf::StringValue * /* resp */ )
{
    phxrpc::log( LOG_ERR, "ERROR: PHXEcho unimplemented" );
    return -1;
}

int AllocsvrService :: GetSeq( const allocsvr::SeqReq & /* req */,
        allocsvr::SeqResp * /* resp */ )
{
    phxrpc::log( LOG_ERR, "ERROR: GetSeq unimplemented" );
    return -1;
}


