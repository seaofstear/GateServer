#include "HttpConnection.h"

HttpConnection::HttpConnection(boost::asio::ip::tcp::socket socket):_socket(std::move(socket)) {

}

void HttpConnection::Start() {
	auto self = shared_from_this();
	boost::beast::http::async_read(_socket, _buffer, _request, [self](boost::beast::error_code ec,std::size_t bytes_transferred) {
		try {
			if (ec) {
				std::cout << "read error:" << ec.message() << std::endl;
				return;
			}
			boost::ignore_unused(bytes_transferred);
			self->HandleReq();
			self->CheckDeadline();
		}
		catch (std::exception& exp) {
			std::cout << "exception error:" << exp.what() << std::endl;
		}
	});
}

