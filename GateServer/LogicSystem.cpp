#include "LogicSystem.h"
#include "HttpConnection.h"

void LogicSystem::RegGet(std::string url, HttpHandler handler) {
	_get_handlers.insert(make_pair(url, handler));
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