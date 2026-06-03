#pragma once
#include "const.h"

class HttpConnection:public std::enable_shared_from_this<HttpConnection>
{
public:
	friend class LogicSystem;
	HttpConnection(boost::asio::ip::tcp::socket socket);
	void Start();

private:
	void CheckDeadline();//检查连接是否超时
	void WriteResponse();
	void HandleReq();
	void PreParseGetParam();
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
	boost::asio::ip::tcp::socket _socket;
	boost::beast::flat_buffer _buffer{ 81920 };
	boost::beast::http::request<boost::beast::http::dynamic_body> _request;
	boost::beast::http::response<boost::beast::http::dynamic_body> _response;
	boost::asio::steady_timer deadline_{
		_socket.get_executor(),std::chrono::seconds(60)
	};
};

