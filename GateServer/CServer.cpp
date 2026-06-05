#include "CServer.h"
#include "HttpConnection.h"
CServer::CServer(boost::asio::io_context& ioc, unsigned short& prot):_ioc(ioc),
_acceptor(ioc,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),prot)),_socket(ioc) {

}

CServer::~CServer() {

}

void CServer::Start() {
	auto self = shared_from_this();
	_acceptor.async_accept(_socket, [self](boost::beast::error_code ec) {
		try {
			if (ec) {
				self->Start();
				return;
			}
			
			//创建新连接，并且创建HTTPconnection类
			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
			//继续监听
			self->Start();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
			self->Start();
		}
		});
}