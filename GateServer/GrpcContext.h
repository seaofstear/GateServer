#pragma once
#include <grpcpp/grpcpp.h>
#include <chrono>

// 统一 RPC 超时封装：ClientContext 每次 RPC 单独创建（gRPC 要求不可跨线程/跨调用共享），
// 构造时统一设置 deadline（默认 3000ms）。
class GrpcContext {
public:
	explicit GrpcContext(int timeout_ms = 3000) {
		_ctx.set_deadline(std::chrono::system_clock::now() +
			std::chrono::milliseconds(timeout_ms));
	}

	grpc::ClientContext* get() { return &_ctx; }

private:
	grpc::ClientContext _ctx;
};
