#pragma once
#include "boxr_logger.h"
#include <exception>
#include <mq/Plugin.h>

#define MACRO_PAUSED_QUERY "${Macro.Paused}"

class BoxrException : public std::runtime_error {
public:
	BoxrException(const std::string& message) : runtime_error(message) {}
};

class InvalidBoxConfigurationException : public BoxrException {
public:
	InvalidBoxConfigurationException(const std::string& message) : BoxrException(message) {}
};

class UnsupportedBoxException : public BoxrException {
public:
	UnsupportedBoxException(const std::string& message) : BoxrException(message) {}
};

class UnsupportedBoxrOperationException : public BoxrException {
public:
	UnsupportedBoxrOperationException(const std::string& message) : BoxrException(message) {}
};

class BoxControl {
public:
	virtual const char* GetKey() = 0;
	virtual const char* GetName() = 0;
	virtual bool IsRunning() = 0;
	virtual void Pause() = 0;
	virtual void Unpause() = 0;
	virtual void Chase() = 0;
	virtual void Camp() = 0;
	virtual void Manual() = 0;
	virtual void BurnNow() = 0;
	virtual void SetRaidAssistNum(int raidAssistNum) = 0;

	virtual std::string GetPauseQuery() = 0;

	virtual ~BoxControl() = default;
};

class RGMercsControl : public BoxControl {
public:
	const char* GetKey() override { return "rgmercs"; }
	const char* GetName() override { return "rgmercs"; }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
	inline std::string GetPauseQuery() { return MACRO_PAUSED_QUERY; }
};

class RGMercsLuaControl : public BoxControl {
public:
	const char* GetKey() override { return "rgmercs.lua"; }
	const char* GetName() override { return "RGMercs - Lua edition"; }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
	inline std::string GetPauseQuery() { return MACRO_PAUSED_QUERY; }
};

class KissAssistControl : public BoxControl {
public:
	const char* GetKey() override { return "kissassist"; }
	const char* GetName() override { return "KissAssist"; }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
	inline std::string GetPauseQuery() { return MACRO_PAUSED_QUERY; }
};

class MuleAssistControl : public KissAssistControl {
public:
	const char* GetKey() override { return "muleassist"; }
	const char* GetName() override { return "MuleAssist"; }
	bool IsRunning() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

class AlsoKissAssistControl : public KissAssistControl {
public:
	const char* GetKey() override { return "alsokissassist"; }
	const char* GetName() override { return "AlsoKissAssist"; }
	bool IsRunning() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

class CwtnControl : public BoxControl {
public:
	const char* GetKey() override { return "cwtn"; }
	const char* GetName() override { return GetClassPlugin(); }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
	inline std::string GetPauseQuery() { return "${CWTN.Paused}"; }

private:
	bool IsClassPluginLoaded();
	const char* GetClassPlugin();
	const char* GetClassCommand();
};

class EntropyControl : public BoxControl {
public:
	const char* GetKey() override { return "entropy"; }
	const char* GetName() override { return "Entropy"; }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
	inline std::string GetPauseQuery() { return MACRO_PAUSED_QUERY; }
};

class XGenControl : public BoxControl {
public:
	const char* GetKey() override { return "xgen"; }
	const char* GetName() override { return "Xgen"; }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
	inline std::string GetPauseQuery() { return MACRO_PAUSED_QUERY; }
};

#define NOOP_MESSAGE "MQ2Boxr does not have support for whatever is running this toon, sorry."
#define LOG_NOOP_WARNING LOGGER.info(NOOP_MESSAGE)
#define THROW_NOOP_EXCEPTION  throw UnsupportedBoxException(NOOP_MESSAGE)
class NoopControl : public BoxControl {
public:
	const char* GetKey() override { return "noop"; }
	const char* GetName() override { return "Nothing"; }
	bool IsRunning() override { return true; }
	void Pause() override { LOG_NOOP_WARNING; };
	void Unpause() override { LOG_NOOP_WARNING; }
	void Chase() override { LOG_NOOP_WARNING; }
	void Camp() override { LOG_NOOP_WARNING; }
	void Manual() override { LOG_NOOP_WARNING; }
	void BurnNow() override { LOG_NOOP_WARNING; }
	void SetRaidAssistNum(int raidAssistNum) override { LOG_NOOP_WARNING; }
	inline std::string GetPauseQuery() { THROW_NOOP_EXCEPTION; }
};

class MasterBoxControl {
public:
	MasterBoxControl(MasterBoxControl const&) = delete;
	void operator=(MasterBoxControl const&) = delete;

	static MasterBoxControl& getInstance() {
		static MasterBoxControl instance;
		return instance;
	}

	void Pause();
	void Unpause();
	void Chase();
	void Camp();
	void Manual();
	void BurnNow();
	void RaidAssistNum(int raidAssistNum);
	bool IsPaused();
	std::string Current();

private:
	MasterBoxControl();
	std::vector<std::shared_ptr<BoxControl>> boxes;
	std::shared_ptr<BoxControl> GetBox();
	std::shared_ptr<NoopControl> noopControl = std::make_shared<NoopControl>();
};
