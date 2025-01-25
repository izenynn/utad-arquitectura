#include "logger.h"

#include <iostream>
#include <string>
#include <chrono>

namespace {
	std::string CurrentDateTimeToString()
	{
		auto now = std::chrono::system_clock::now();
		auto now_time_t = std::chrono::system_clock::to_time_t(now);
		struct tm now_tm{};
#ifdef _WIN32
		localtime_s(&now_tm, &now_time_t);
#else
		localtime_r(&now_time_t, &now_tm);
#endif

		char buffer[30];
		std::strftime(
				buffer,
				sizeof(buffer) / sizeof(char),
				"%Y-%m-%d %H:%M:%S",
				&now_tm);
		return std::string{buffer};
	}
} // namespace

std::vector<LogEntry> Logger::messages_{};

void Logger::Info(const std::string& message)
{
	LogEntry entry;
	entry.type = LogType::kLogInfo;
	entry.message = "[" + CurrentDateTimeToString() + "] [INFO]: " + message;

	std::cout << "\x1B[32m" << entry.message << "\033[0m" << std::endl;
	messages_.push_back(entry);
}

void Logger::Warn(const std::string& message)
{
	LogEntry entry;
	entry.type = LogType::kLogInfo;
	entry.message = "[" + CurrentDateTimeToString() + "] [INFO]: " + message;

	std::cout << "\x1B[93m" << entry.message << "\033[0m" << std::endl;
	messages_.push_back(entry);
}

void Logger::Error(const std::string& message)
{
	LogEntry entry;
	entry.type = LogType::kLogError;
	entry.message = "[" + CurrentDateTimeToString() + "] [ERORR]: " + message;

	std::cerr << "\x1B[91m" << entry.message << "\033[0m" << std::endl;
	messages_.push_back(entry);
}
