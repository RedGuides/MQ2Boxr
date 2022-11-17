#include <mq/Plugin.h>
#include "boxr.h"
#include "boxr_type.h"


MQ2BoxrType::MQ2BoxrType() : MQ2Type(BOXR_TYPE_NAME) {
	ScopedTypeMember(Members, Paused);
	ScopedTypeMember(Members, Current);
}

void MQ2BoxrType::RegisterBoxrType() {
	AddMQ2Data(BOXR_TYPE_NAME, &MQ2BoxrType::BoxrData);
}

void MQ2BoxrType::UnregisterBoxrType() {
	RemoveMQ2Data(BOXR_TYPE_NAME);
}

bool MQ2BoxrType::BoxrData(const char* Index, MQTypeVar& Dest) {
	Dest.DWord = 1;
	Dest.Type = &MQ2BoxrType::getInstance();
	return true;
}

bool MQ2BoxrType::GetMember(MQVarPtr _ignoredVarPtr, const char* Member, char* _ignoredIndex, MQTypeVar& Dest) {
	auto pMember = MQ2BoxrType::FindMember(Member);
	if (pMember == nullptr)
		return false;

	switch (static_cast<Members>(pMember->ID)) {
		case Members::Current: {
			return GetCurrentBoxKey(Dest);
		}
		case Members::Paused: {
			return GetPauseState(Dest);
		}
		default: {
			return false;
		}
	}
}

bool MQ2BoxrType::GetCurrentBoxKey(mq::MQTypeVar& Dest) {
	strcpy_s(DataTypeTemp, MasterBoxControl::getInstance().Current().c_str());
	Dest.Type = mq::datatypes::pStringType;
	Dest.Ptr = DataTypeTemp;
	return true;
}

bool MQ2BoxrType::GetPauseState(mq::MQTypeVar& Dest) {
	try {
		Dest.Type = mq::datatypes::pBoolType;
		Dest.Set(MasterBoxControl::getInstance().IsPaused());
		return true;
	} catch (BoxrException& e) {
		LOGGER.error(e.what());
		Dest.Type = mq::datatypes::pBoolType;
		Dest.Set(nullptr);
		return false;
	}
}
