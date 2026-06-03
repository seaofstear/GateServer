#pragma once
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <chrono>
#include <iostream>
#include "Singleton.h"
#include <vector>
#include <thread>
#include <functional>
#include <map>
#include <unordered_map>
#include "nlohmann/json.hpp"

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,
	RPCFailed = 1002,
};;