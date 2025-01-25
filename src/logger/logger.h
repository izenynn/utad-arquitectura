#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <vector>

enum class LogType {
	kLogInfo = 0,
	kLogWarn,
	kLogError
};

struct LogEntry {
	LogType type{LogType::kLogInfo};
	std::string message{};
};

class Logger {
public:
	static void Info(const std::string& message);
	static void Warn(const std::string& message);
	static void Error(const std::string& message);

private:
	static std::vector<LogEntry> messages_;
};

#endif // LOGGER_H_