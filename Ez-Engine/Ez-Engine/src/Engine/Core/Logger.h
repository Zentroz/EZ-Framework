#ifndef LOGGER_CLASS_H
#define LOGGER_CLASS_H

#include<vector>
#include<string>

namespace ENGINE {
	enum LogType { Warning, Info, Error };
	struct Log {
		LogType type;
		std::string msg;

		Log(LogType type, std::string msg) : msg(msg), type(type) {}
	};

	class Logger {
	public:
		Logger() = delete;

		static void LogMessage(Log log);
		static std::vector<Log>& GetLogs() { return logs; }
		static void Clear() { logs.clear(); }

	private:
		static std::vector<Log> logs;
	};
}

#define LOG_INFO(msg)    ENGINE::Logger::LogMessage(ENGINE::Log(ENGINE::LogType::Info, msg))
#define LOG_WARN(msg)    ENGINE::Logger::LogMessage(ENGINE::Log(ENGINE::LogType::Warning, msg))
#define LOG_ERROR(msg)   ENGINE::Logger::LogMessage(ENGINE::Log(ENGINE::LogType::Error, msg))

#endif