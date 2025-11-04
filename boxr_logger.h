#pragma once
#include <string_view>
#include <mq/Plugin.h>
#include <fmt/format.h>

#define LOGGER BoxrLogger::getInstance()
#define BOXR_LOG_HEADER "\a-t[\atMQ2Boxr\ax] "

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
	void error(fmt::format_string<Args...> fmt, Args&&... args) {
		doLog(errorFormat, fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void info(fmt::format_string<Args...> fmt, Args&&... args) {
		doLog(infoFormat, fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void debug(fmt::format_string<Args...> fmt, Args&&... args) {
		if (!debugEnabled) {
			return;
		}
		doLog(debugFormat, fmt, std::forward<Args>(args)...);
	}

private:
	BoxrLogger() = default;
	bool debugEnabled = false;

	const char* infoFormat = BOXR_LOG_HEADER "%s";
	const char* debugFormat = BOXR_LOG_HEADER "\a-y[\ayDEBUG\a-y] \ao %s";
	const char* errorFormat = BOXR_LOG_HEADER "\a-r[\arERROR\ax] \ao %s";

	template<typename... Args>
	void doLog(const char* logFormat, fmt::format_string<Args...> fmt, Args&&... args) {
		auto message = fmt::vformat(fmt, fmt::make_format_args(args...));
		WriteChatf(logFormat, message.c_str());
	}
};
