#pragma once

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"
#include <memory>
using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

// 共享单 Channel + Stub 模式：Channel/Stub 线程安全、可跨线程复用，
// ClientContext 每次 RPC 单独创建（统一 deadline 见 GrpcContext）。
class VerifyGrpcClient :public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;
public:
    GetVarifyRsp GetVarifyCode(std::string email);

private:
    VerifyGrpcClient();

    std::shared_ptr<grpc::Channel> _channel;
    std::shared_ptr<VarifyService::Stub> _stub;
};
