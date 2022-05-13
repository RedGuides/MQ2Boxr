#pragma once
#include "boxr_logger.h"
#include <mq/Plugin.h>

bool isClassPluginLoaded();

const char* getClassPlugin();

class BoxControl {
public:
	virtual const char* GetName() = 0;
	virtual bool isRunning() = 0;
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
	bool isRunning() override;
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
	bool isRunning() override;
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
	bool isRunning() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

class AlsoKissAssistControl : public KissAssistControl {
public:
	const char* GetName() override { return "AlsoKissAssist"; }
	bool isRunning() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

class CwtnControl : public BoxControl {
public:
	const char* GetName() override { return getClassPlugin(); }
	bool isRunning() override;
	void Pause() override;
	void Unpause() override;
	void Chase() override;
	void Camp() override;
	void Manual() override;
	void BurnNow() override;
	void SetRaidAssistNum(int raidAssistNum) override;
};

class EntropyControl : public BoxControl {
public:
	const char* GetName() override { return "Entropy"; }
	bool isRunning() override;
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
	bool isRunning() override;
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
	bool isRunning() override { return true; }
	void Pause() override { LOG_NOOP_WARNING; };
	void Unpause() override { LOG_NOOP_WARNING; }
	void Chase() override { LOG_NOOP_WARNING; }
	void Camp() override { LOG_NOOP_WARNING; }
	void Manual() override { LOG_NOOP_WARNING; }
	void BurnNow() override { LOG_NOOP_WARNING; }
	void SetRaidAssistNum(int raidAssistNum) override { LOG_NOOP_WARNING; }
};

typedef void (BoxControl::* BoxControlMemFn)();

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
	std::shared_ptr<BoxControl> getBox();
	std::shared_ptr<NoopControl> noopControl = std::make_shared<NoopControl>();
};
