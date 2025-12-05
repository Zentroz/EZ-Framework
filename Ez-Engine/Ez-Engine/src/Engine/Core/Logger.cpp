#include"Logger.h"

namespace ENGINE {
	std::vector<Log> Logger::logs = {};

	void Logger::LogMessage(Log log) {
		Logger::logs.push_back(log);
	}
}