#include "boxr.h"
#include <string>

#define SQUELCH(CMD, ...) DoCommandf("/squelch " CMD, __VA_ARGS__)

bool debugEnabled = false;

void MasterBoxControl::Pause() {
	auto box = getBox();
	LOG("Pausing %s", box->GetName());
	box->Pause();
}

void MasterBoxControl::Unpause() {
	auto box = getBox();
	LOG("Unpausing %s", box->GetName());
	box->Unpause();
}

void MasterBoxControl::Chase() {
	LOG("Setting \ayCHASE\ax mode");
	getBox()->Chase();
}

void MasterBoxControl::Camp() {
	LOG("Setting \ayCAMP\ax mode");
	getBox()->Camp();
}

void MasterBoxControl::Manual() {
	LOG("Setting \ayMANUAL\ax mode");
	getBox()->Manual();
}

void MasterBoxControl::CampRadius(int campRadius) {
	LOG("Setting \a-tcamp radius\ax to \at%d\ax", campRadius);
	getBox()->SetCampRadius(campRadius);
}

void MasterBoxControl::RaidAssistNum(int raidAssistNum) {
	LOG("Setting \a-tRaidAssistNum\ax to \at%d\ax (\at%s\ax)", raidAssistNum,
		GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
	LOG_DEBUG("RaidAssist 1: %s", GetCharInfo()->raidData.MainAssistNames[0]);
	LOG_DEBUG("RaidAssist 2: %s", GetCharInfo()->raidData.MainAssistNames[1]);
	LOG_DEBUG("RaidAssist 3: %s", GetCharInfo()->raidData.MainAssistNames[2]);
	getBox()->SetRaidAssistNum(raidAssistNum);
}

bool stringStartsWith(const char* pre, const char* str) {
	return strncmp(pre, str, strlen(pre)) == 0;
}

BoxControl* MasterBoxControl::getBox() {
	// Assume that if a macro is running, it is controlling the character, even if
	// a the class's CWTN plugin is loaded (since otherwise, why start the macro?)
	if (gMacroStack && strlen(gszMacroName) && stringStartsWith("kiss", gszMacroName)) {
		return &kissAssistControl;
	}
	else if (gMacroStack && strlen(gszMacroName) && stringStartsWith("rgmercs", gszMacroName)) {
		return &rgmercsControl;
	}
	else if (isClassPluginLoaded()) {
		return &cwtnControl;
	}
	else {
		return &noopControl;
	}
}

void pauseTwist() {
	if (GetCharInfo2()->Class == Bard) {
		SQUELCH("/twist off");
	}
}

void RGMercsControl::Pause() {
	SQUELCH("/mqp on");
	pauseTwist();
}

void RGMercsControl::Unpause() {
	SQUELCH("/mqp off");
}

void RGMercsControl::Chase() {
	SQUELCH("/rg chaseon");
}

void RGMercsControl::Camp() {
	SQUELCH("/rg camphard");
}

void RGMercsControl::Manual() {
	SQUELCH("/rg chaseoff");
	SQUELCH("/timed 1 /rg campoff");
}

void RGMercsControl::SetCampRadius(int campRadius) {
	SQUELCH("/rg autocampradius %d", campRadius);
}

void RGMercsControl::SetRaidAssistNum(int raidAssistNum) {
	SQUELCH("/rg AssistOutside 1");
	SQUELCH("/timed 1 /rg OutsideAssistList %s", GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
}

void KissAssistControl::Pause() {
	SQUELCH("/mqp on");
	pauseTwist();
}

void KissAssistControl::Unpause() {
	SQUELCH("/mqp off");
}

void KissAssistControl::Chase() {
	SQUELCH("/chaseon");
}

void KissAssistControl::Camp() {
	SQUELCH("/camphere on");
}

void KissAssistControl::Manual() {
	SQUELCH("/chaseoff");
	// Macros seem to sometimes miss the second command if they are run
	// in too quick succession on slower systems, thus the /timed here
	SQUELCH("/timed 1 /camphere off ");
}

void KissAssistControl::SetCampRadius(int campRadius) {
	SQUELCH("/campradius %d", campRadius);
}

void KissAssistControl::SetRaidAssistNum(int raidAssistNum) {
	SQUELCH("/switchma %s tank 1", GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
}

const char* getPlayerClassAbbr() {
	return ClassInfo[GetCharInfo2()->Class].ShortName;
}

void CwtnControl::Pause() {
	SQUELCH("/%s pause on", getPlayerClassAbbr());
}

void CwtnControl::Unpause() {
	SQUELCH("/%s pause off", getPlayerClassAbbr());
}

void CwtnControl::Chase() {
	SQUELCH("/%s mode chase", getPlayerClassAbbr());
}

void CwtnControl::Camp() {
	SQUELCH("/%s mode assist", getPlayerClassAbbr());
	SQUELCH("/%s resetcamp", getPlayerClassAbbr());
}

void CwtnControl::Manual() {
	SQUELCH("/%s mode manual", getPlayerClassAbbr());
}

void CwtnControl::SetCampRadius(int campRadius) {
	SQUELCH("/%s campradius %d", getPlayerClassAbbr(), campRadius);
}

void CwtnControl::SetRaidAssistNum(int raidAssistNum) {
	SQUELCH("/%s raidassistnum %d", getPlayerClassAbbr(), raidAssistNum);
}

bool isPluginLoaded(const char* pluginName) {
	PMQPLUGIN pPlugin = pPlugins;
	while (pPlugin) {
		if (!_stricmp(pluginName, pPlugin->szFilename)) {
			return true;
		}
		pPlugin = pPlugin->pNext;
	}
	return false;
}

bool isClassPluginLoaded() {
	auto classPluginName = getClassPlugin();
	if (classPluginName == nullptr) {
		return false;
	}
	return isPluginLoaded(classPluginName);
}

const char* getClassPlugin() {
	switch (GetCharInfo2()->Class) {
	case Beastlord:
		return "MQ2Bst";
	case Berserker:
		return "MQ2BerZerker";
	case Cleric:
		return "MQ2Cleric";
	case Enchanter:
		return "MQ2Enchanter";
	case Monk:
		return "MQ2Monk";
	case Rogue:
		return "MQ2Rogue";
	case Shadowknight:
		return "MQ2Eskay";
	case Shaman:
		return "MQ2Shaman";
	case Warrior:
		return "MQ2War";
	default:
		return nullptr;
	}
}
