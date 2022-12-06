#pragma once
#include <mq/Plugin.h>

constexpr auto BOXR_TYPE_NAME = "Boxr";

class MQ2BoxrType : public MQ2Type {
public:
	MQ2BoxrType(MQ2BoxrType const&) = delete;
	void operator=(MQ2BoxrType const&) = delete;

	static void RegisterBoxrType();
	static void UnregisterBoxrType();

	static bool BoxrData(const char* Index, MQTypeVar& Dest);

private:
	MQ2BoxrType();
	enum class Members {
		Paused,
		Current
	};

	static MQ2BoxrType& getInstance() {
		static MQ2BoxrType instance;
		return instance;
	}

	virtual bool GetMember(MQVarPtr _ignoredVarPtr, const char* Member, char* _ignoredIndex, MQTypeVar& Dest) override;
	bool GetCurrentBoxKey(mq::MQTypeVar& Dest);
	bool GetPauseState(mq::MQTypeVar& Dest);
};
