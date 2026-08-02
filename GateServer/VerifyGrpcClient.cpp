#include "VerifyGrpcClient.h"
#include "ConfigMgr.h"
#include "GrpcContext.h"
#include <iostream>

VerifyGrpcClient::VerifyGrpcClient() {
	auto& gCfgMgr = ConfigMgr::Inst();
	std::string host = gCfgMgr["VarifyServer"]["Host"];
	std::string port = gCfgMgr["VarifyServer"]["Port"];
	_channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
	_stub = VarifyService::NewStub(_channel);
}

GetVarifyRsp VerifyGrpcClient::GetVarifyCode(std::string email) {
	GetVarifyRsp reply;
	GetVarifyReq request;
	request.set_email(email);
	GrpcContext context;
	Status status = _stub->GetVarifyCode(context.get(), request, &reply);
	if (status.ok()) {
		return reply;
	}

	std::cerr << "VerifyGrpcClient::GetVarifyCode failed, error_code is " << status.error_code()
		<< ", error_message is " << status.error_message() << std::endl;
	reply.set_error(ErrorCodes::RPCFailed);
	return reply;
}
