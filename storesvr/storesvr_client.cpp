/* storesvr_client.cpp

 Generated by phxrpc_pb2client from storesvr.proto

*/

#include <iostream>
#include <memory>
#include <stdlib.h>
#include <mutex>

#include "storesvr_client.h"
#include "phxrpc_storesvr_stub.h"

#include "phxrpc/rpc.h"

static phxrpc::ClientConfig global_storesvrclient_config_;
static phxrpc::ClientMonitorPtr global_storesvrclient_monitor_;

bool StoresvrClient :: Init( const char * config_file )
{
    return global_storesvrclient_config_.Read( config_file );
}

const char * StoresvrClient :: GetPackageName() {
    const char * ret = global_storesvrclient_config_.GetPackageName();
    if (strlen(ret) == 0) {
        ret = "storesvr";
    }
    return ret;
}

StoresvrClient :: StoresvrClient()
{
    static std::mutex monitor_mutex;
    if ( !global_storesvrclient_monitor_.get() ) { 
        monitor_mutex.lock();
        if ( !global_storesvrclient_monitor_.get() ) {
            global_storesvrclient_monitor_ = phxrpc::MonitorFactory::GetFactory()
                ->CreateClientMonitor( GetPackageName() );
        }
        global_storesvrclient_config_.SetClientMonitor( global_storesvrclient_monitor_ );
        monitor_mutex.unlock();
    }
}

StoresvrClient :: ~StoresvrClient()
{
}

int StoresvrClient :: PHXEcho( const google::protobuf::StringValue & req,
        google::protobuf::StringValue * resp )
{
    const phxrpc::Endpoint_t * ep = global_storesvrclient_config_.GetRandom();

    if(ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_storesvrclient_config_.GetConnectTimeoutMS(), NULL, 0, 
                    *(global_storesvrclient_monitor_.get()));
        if ( open_ret ) {
            socket.SetTimeout(global_storesvrclient_config_.GetSocketTimeoutMS());

            StoresvrStub stub(socket, *(global_storesvrclient_monitor_.get()));
            return stub.PHXEcho(req, resp);
        } 
    }

    return -1;
}

int StoresvrClient :: PhxBatchEcho( const google::protobuf::StringValue & req,
        google::protobuf::StringValue * resp )
{
    int ret = -1; 
    size_t echo_server_count = 2;
    uthread_begin;
    for (size_t i = 0; i < echo_server_count; i++) {
        uthread_t [=, &uthread_s, &ret](void *) {
            const phxrpc::Endpoint_t * ep = global_storesvrclient_config_.GetByIndex(i);
            if (ep != nullptr) {
                phxrpc::UThreadTcpStream socket;
                if(phxrpc::PhxrpcTcpUtils::Open(&uthread_s, &socket, ep->ip, ep->port,
                            global_storesvrclient_config_.GetConnectTimeoutMS(), *(global_storesvrclient_monitor_.get()))) { 
                    socket.SetTimeout(global_storesvrclient_config_.GetSocketTimeoutMS());
                    StoresvrStub stub(socket, *(global_storesvrclient_monitor_.get()));
                    int this_ret = stub.PHXEcho(req, resp);
                    if (this_ret == 0) {
                        ret = this_ret;
                        uthread_s.Close();
                    }   
                }   
            }
        };  
    }   
    uthread_end;
    return ret;
}

int StoresvrClient :: GetRouterList( const storesvr::RouterReq & req,
        storesvr::RouterResp * resp )
{
    const phxrpc::Endpoint_t * ep = global_storesvrclient_config_.GetRandom();

    if(ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_storesvrclient_config_.GetConnectTimeoutMS(), NULL, 0, 
                    *(global_storesvrclient_monitor_.get()));
        if ( open_ret ) {
            socket.SetTimeout(global_storesvrclient_config_.GetSocketTimeoutMS());

            StoresvrStub stub(socket, *(global_storesvrclient_monitor_.get()));
            return stub.GetRouterList(req, resp);
        } 
    }

    return -1;
}

int StoresvrClient :: GetSections( const storesvr::SectionReq & req,
        storesvr::SectionResp * resp )
{
    const phxrpc::Endpoint_t * ep = global_storesvrclient_config_.GetRandom();

    if(ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_storesvrclient_config_.GetConnectTimeoutMS(), NULL, 0, 
                    *(global_storesvrclient_monitor_.get()));
        if ( open_ret ) {
            socket.SetTimeout(global_storesvrclient_config_.GetSocketTimeoutMS());

            StoresvrStub stub(socket, *(global_storesvrclient_monitor_.get()));
            return stub.GetSections(req, resp);
        } 
    }

    return -1;
}

int StoresvrClient :: storeSection( const storesvr::StoreReq & req,
        storesvr::StoreResp * resp )
{
    const phxrpc::Endpoint_t * ep = global_storesvrclient_config_.GetRandom();

    if(ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_storesvrclient_config_.GetConnectTimeoutMS(), NULL, 0, 
                    *(global_storesvrclient_monitor_.get()));
        if ( open_ret ) {
            socket.SetTimeout(global_storesvrclient_config_.GetSocketTimeoutMS());

            StoresvrStub stub(socket, *(global_storesvrclient_monitor_.get()));
            return stub.storeSection(req, resp);
        } 
    }

    return -1;
}


