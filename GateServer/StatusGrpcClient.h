#pragma once
#include "const.h"
#include "Singleton.h"
#include "ConfigMgr.h"
#include <grpcpp/grpcpp.h> 
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <memory>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::LoginRsp;
using message::LoginReq;
using message::StatusService;

// 共享单 Channel + Stub 模式：Channel/Stub 线程安全、可跨线程复用，
// ClientContext 每次 RPC 单独创建（统一 deadline 见 GrpcContext）。
class StatusGrpcClient :public Singleton<StatusGrpcClient>
{
	friend class Singleton<StatusGrpcClient>;
public:
	~StatusGrpcClient() {

	}
	GetChatServerRsp GetChatServer(int uid);
	//LoginRsp Login(int uid, std::string token);
private:
	StatusGrpcClient();
	std::shared_ptr<grpc::Channel> _channel;
	std::shared_ptr<StatusService::Stub> _stub;

};
