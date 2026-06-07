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
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "hiredis.h"
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "hiredis.h"
#include <cassert>

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,
	RPCFailed = 1002,
};;
