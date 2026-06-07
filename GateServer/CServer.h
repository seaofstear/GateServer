#pragma once
#include <boost/beast/http.hpp>
#include "const.h"


class CServer:public std::enable_shared_from_this<CServer>
{
public:
	CServer(boost::asio::io_context& ioc, unsigned short& prot);
	~CServer();
	void Start();
private:
	boost::asio::ip::tcp::acceptor _acceptor;
	boost::beast::net::io_context& _ioc;

};

