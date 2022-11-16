#pragma once
#include "boxr_logger.h"
#include <exception>
#include <mq/Plugin.h>

class BoxControl {
public:
	virtual const char* GetName() = 0;
	virtual bool IsRunning() = 0;
	virtual void Pause() = 0;
	virtual void Unpause() = 0;
	virtual void Chase() = 0;
	virtual void Camp() = 0;
	virtual void Manual() = 0;
	virtual void BurnNow() = 0;
	virtual void SetRaidAssistNum(int raidAssistNum) = 0;

	virtual ~BoxControl() = default;
};

class RGMercsControl : public BoxControl {
public:
	const char* GetName() override { return "rgmercs"; }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

class KissAssistControl : public BoxControl {
public:
	const char* GetName() override { return "KissAssist"; }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

class MuleAssistControl : public KissAssistControl {
public:
	const char* GetName() override { return "MuleAssist"; }
	bool IsRunning() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

class AlsoKissAssistControl : public KissAssistControl {
public:
	const char* GetName() override { return "AlsoKissAssist"; }
	bool IsRunning() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

class CwtnControl : public BoxControl {
public:
	const char* GetName() override { return GetClassPlugin(); }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;

private:
	bool IsClassPluginLoaded();
	const char* GetClassPlugin();
	const char* GetClassCommand();
};

class EntropyControl : public BoxControl {
public:
	const char* GetName() override { return "Entropy"; }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

class XGenControl : public BoxControl {
public:
	const char* GetName() override { return "Xgen"; }
	bool IsRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

#define LOG_NOOP_WARNING LOGGER.info("MQ2Boxr does not have support for whatever is running this toon, sorry.")
class NoopControl : public BoxControl {
public:
	const char* GetName() override { return "Nothing that is recognized by MQ2Boxr"; }
	bool IsRunning() override { return true; }
	void Pause() override { LOG_NOOP_WARNING; };
	void Unpause() override { LOG_NOOP_WARNING; }
	void Chase() override { LOG_NOOP_WARNING; }
	void Camp() override { LOG_NOOP_WARNING; }
	void Manual() override { LOG_NOOP_WARNING; }
	void BurnNow() override { LOG_NOOP_WARNING; }
	void SetRaidAssistNum(int raidAssistNum) override { LOG_NOOP_WARNING; }
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

private:
	MasterBoxControl();
	std::vector<std::shared_ptr<BoxControl>> boxes;
	std::shared_ptr<BoxControl> GetBox();
	std::shared_ptr<NoopControl> noopControl = std::make_shared<NoopControl>();
};
