#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifyGrpcClient.h"

void LogicSystem::RegGet(std::string url, HttpHandler handler) {
	_get_handlers.insert(make_pair(url, handler));
}

void LogicSystem::RegPorst(std::string url, HttpHandler handler) {
    _post_handlers.insert(make_pair(url, handler));
}


LogicSystem::LogicSystem() {
    RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
        boost::beast::ostream(connection->_response.body()) << "receive get_test req " << std::endl;
        int i = 0;
        for (auto& elem : connection->_get_params) {
            i++;
            boost::beast::ostream(connection->_response.body()) << "param" << i << " key is " << elem.first;
            boost::beast::ostream(connection->_response.body()) << ", " << " value is " << elem.second << std::endl;
        }
        });

    RegPorst("/get_varifycode",[](std::shared_ptr<HttpConnection> connection) {
		auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
		std::cout << "receive post req get_varifycode, body is " << body_str << std::endl;
		connection->_response.set(boost::beast::http::field::content_type, "text/json");
        nlohmann::json root;
        nlohmann::json src_root;
        src_root = nlohmann::json::parse(body_str,nullptr,false);
        if (src_root.is_discarded()) {
			std::cout << "json parse error" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.dump();
			boost::beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        

        if (!src_root.contains("email"))
        {
            std::cout << "json parse error" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string jsonstr = root.dump();
            boost::beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        auto email = src_root["email"].get<std::string>();
        GetVarifyRsp rsp = VerifyGrpcClient::GetInstance()->GetVarifyCode(email);
		std::cout << "email is " << email << std::endl;
		root["error"] = rsp.error();
		root["email"] = src_root["email"];
		std::string jsonstr = root.dump();
		boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
		});
}


bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con) {
    if (_get_handlers.find(path) == _get_handlers.end()) {
        return false;
    }

    _get_handlers[path](con);
    return true;
}

LogicSystem::~LogicSystem() {
	std::cout << "LogicSystem destruct" << std::endl;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con) {
    if (_post_handlers.find(path) == _post_handlers.end()) {
        return false;
    }
    _post_handlers[path](con);
    return true;
}