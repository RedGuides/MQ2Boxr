#pragma once
#include <string_view>
#include <mq/Plugin.h>
#include <fmt/format.h>

#define LOG_HEADER "\a-t[\atMQ2Boxr\ax] "

class BoxrLogger {
public:
	BoxrLogger(BoxrLogger const&) = delete;
	void operator=(BoxrLogger const&) = delete;

	static BoxrLogger& getInstance() {
		static BoxrLogger instance;
		return instance;
	}

	void setDebugEnabled(bool debugEnabled) {
		this->debugEnabled = debugEnabled;
	}

	bool isDebugEnabled() {
		return debugEnabled;
	}

	void toggleDebugEnabled() {
		debugEnabled = !debugEnabled;
	}

	template <typename... Args>
	void error(std::string_view messageFormat, Args ...args) {
		doLog(errorFormat, messageFormat, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void info(std::string_view messageFormat, Args ...args) {
		doLog(infoFormat, messageFormat, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void debug(std::string_view messageFormat, Args ...args) {
		if (!debugEnabled) {
			return;
		}
		doLog(debugFormat, messageFormat, std::forward<Args>(args)...);
	}

private:
	BoxrLogger() {};
	bool debugEnabled = false;

	static constexpr char* infoFormat = LOG_HEADER "%s";
	static constexpr char* debugFormat = LOG_HEADER "\a-y[\ayDEBUG\a-y] \ao %s";
	static constexpr char* errorFormat = LOG_HEADER "\a-r[\arERROR\ax] \ao %s";

	template<typename ...Args>
	void doLog(const char* logFormat, std::string_view messageFormat, Args ...args) {
		auto message = fmt::format(messageFormat, std::forward<Args>(args)...);
		WriteChatf(logFormat, message.c_str());
	}
};
