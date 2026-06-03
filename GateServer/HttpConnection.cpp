#include "HttpConnection.h"
#include "LogicSystem.h"
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

void HttpConnection::HandleReq() {
	_response.version(_request.version());
	_response.keep_alive(false);
	if(_request.method() == boost::beast::http::verb::get) {
		bool success = LogicSystem::GetInstance()->HandleGet(_request.target(), shared_from_this());
		if (!success) {
			_response.result(boost::beast::http::status::not_found);
			_response.set(boost::beast::http::field::content_type, "text/html");
			boost::beast::ostream(_response.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}

		_response.result(boost::beast::http::status::ok);
		_response.set(boost::beast::http::field::server, "GateServer");
		WriteResponse();
		return;
	}
	else {
		_response.result(boost::beast::http::status::bad_request);
	}

}

void HttpConnection::WriteResponse() {
	auto self = shared_from_this();
	_request.content_length(_response.body().size());
	boost::beast::http::async_write(_socket, _response, [self](boost::beast::error_code ec, std::size_t bytes_transferred) {
		self->_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
		self->deadline_.cancel();
		});
}

void HttpConnection::CheckDeadline() {
	auto self = shared_from_this();
	deadline_.async_wait([self](boost::beast::error_code ec) {
		if (!ec) {
			self->_socket.close(ec);
		}
	});
}

