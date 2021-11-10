#include "boxr.h"

#define SQUELCH(CMD, ...) DoCommandf("/squelch " CMD, __VA_ARGS__);
#define DEBUG_LOG_AND_RUN(CMD, ...) LOG_DEBUG("Running command: " CMD, __VA_ARGS__); DoCommandf(CMD, __VA_ARGS__);
#define BOXR_RUN_COMMANDF(...) if (debugEnabled) { DEBUG_LOG_AND_RUN(__VA_ARGS__) } else  { SQUELCH(__VA_ARGS__) }

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

void MasterBoxControl::BurnNow() {
	LOG("Burn phase NOW!");
	getBox()->BurnNow();
}

void MasterBoxControl::RaidAssistNum(int raidAssistNum) {
	LOG("Setting \a-tRaidAssistNum\ax to \at%d\ax (\at%s\ax)", raidAssistNum,
		GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
	LOG_DEBUG("RaidAssist 1: %s", GetCharInfo()->raidData.MainAssistNames[0]);
	LOG_DEBUG("RaidAssist 2: %s", GetCharInfo()->raidData.MainAssistNames[1]);
	LOG_DEBUG("RaidAssist 3: %s", GetCharInfo()->raidData.MainAssistNames[2]);
	getBox()->SetRaidAssistNum(raidAssistNum);
}

MasterBoxControl::MasterBoxControl() {
	// Assume that if a macro is running, it is controlling the character, even if
	// a the class's CWTN plugin is loaded (since otherwise, why start the macro?)
	boxes.push_back(std::make_shared<RGMercsControl>());
	boxes.push_back(std::make_shared<KissAssistControl>());
	boxes.push_back(std::make_shared<MuleAssistControl>());
	boxes.push_back(std::make_shared<EntropyControl>());
	boxes.push_back(std::make_shared<CwtnControl>());
}

bool stringStartsWith(const char* pre, const char* str) {
	return _strnicmp(pre, str, strlen(pre)) == 0;
}

std::shared_ptr<BoxControl> MasterBoxControl::getBox() {
	for (std::shared_ptr<BoxControl> box : boxes) {
		if (box->isRunning()) {
			LOG_DEBUG("Detected running: %s", box->GetName());
			return box;
		}
	}
	return this->noopControl;
}

void pauseTwist() {
	if (GetPcProfile()->Class == Bard) {
		BOXR_RUN_COMMANDF("/twist off");
	}
}

bool RGMercsControl::isRunning() {
	return stringStartsWith("rgmercs", gszMacroName);
}

void RGMercsControl::Pause() {
	BOXR_RUN_COMMANDF("/mqp on");
	pauseTwist();
}

void RGMercsControl::Unpause() {
	BOXR_RUN_COMMANDF("/mqp off");
}

void RGMercsControl::Chase() {
	BOXR_RUN_COMMANDF("/rg chaseon");
}

void RGMercsControl::Camp() {
	BOXR_RUN_COMMANDF("/rg camphard");
}

void RGMercsControl::Manual() {
	BOXR_RUN_COMMANDF("/rg chaseoff");
	BOXR_RUN_COMMANDF("/timed 1 /rg campoff");
}

void RGMercsControl::BurnNow() {
	LOG("BurnNow is not supported for rgmercs");
}

void RGMercsControl::SetRaidAssistNum(int raidAssistNum) {
	BOXR_RUN_COMMANDF("/rg AssistOutside 1");
	BOXR_RUN_COMMANDF("/timed 1 /rg OutsideAssistList %s", GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
}

bool KissAssistControl::isRunning() {
	return stringStartsWith("kiss", gszMacroName);
}

void KissAssistControl::Pause() {
	BOXR_RUN_COMMANDF("/mqp on");
	pauseTwist();
}

void KissAssistControl::Unpause() {
	BOXR_RUN_COMMANDF("/mqp off");
}

void KissAssistControl::Chase() {
	BOXR_RUN_COMMANDF("/chaseon");
}

void KissAssistControl::Camp() {
	BOXR_RUN_COMMANDF("/camphere on");
}

void KissAssistControl::Manual() {
	BOXR_RUN_COMMANDF("/chaseoff");
	// Macros seem to sometimes miss the second command if they are run
	// in too quick succession on slower systems, thus the /timed here
	BOXR_RUN_COMMANDF("/timed 1 /camphere off ");
}

void KissAssistControl::BurnNow() {
	BOXR_RUN_COMMANDF("/burn");
}

void KissAssistControl::SetRaidAssistNum(int raidAssistNum) {
	BOXR_RUN_COMMANDF("/switchma %s tank 1", GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
}

bool MuleAssistControl::isRunning() {
	return stringStartsWith("muleassist", gszMacroName);
}

void MuleAssistControl::SetRaidAssistNum(int raidAssistNum) {
	BOXR_RUN_COMMANDF("/changema %s", GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
}

const char* getPlayerClassAbbr() {
	return ClassInfo[GetPcProfile()->Class].ShortName;
}

bool CwtnControl::isRunning() {
	return isClassPluginLoaded();
}

void CwtnControl::Pause() {
	BOXR_RUN_COMMANDF("/%s pause on", getPlayerClassAbbr());
}

void CwtnControl::Unpause() {
	BOXR_RUN_COMMANDF("/%s pause off", getPlayerClassAbbr());
}

void CwtnControl::Chase() {
	BOXR_RUN_COMMANDF("/%s mode chase", getPlayerClassAbbr());
}

void CwtnControl::Camp() {
	BOXR_RUN_COMMANDF("/%s mode assist", getPlayerClassAbbr());
	BOXR_RUN_COMMANDF("/%s resetcamp", getPlayerClassAbbr());
}

void CwtnControl::Manual() {
	BOXR_RUN_COMMANDF("/%s mode manual", getPlayerClassAbbr());
}

void CwtnControl::BurnNow() {
	BOXR_RUN_COMMANDF("/%s BurnNow", getPlayerClassAbbr());
}

void CwtnControl::SetRaidAssistNum(int raidAssistNum) {
	BOXR_RUN_COMMANDF("/%s raidassistnum %d", getPlayerClassAbbr(), raidAssistNum);
}

bool EntropyControl::isRunning() {
	return stringStartsWith("entropy", gszMacroName);
}

void EntropyControl::Pause() {
	BOXR_RUN_COMMANDF("/mqp on");
}

void EntropyControl::Unpause() {
	BOXR_RUN_COMMANDF("/mqp off");
}

void EntropyControl::Chase() {
	BOXR_RUN_COMMANDF("/tie on");
}

void EntropyControl::Camp() {
	BOXR_RUN_COMMANDF("/tie off");
	BOXR_RUN_COMMANDF("/timed 1 /home set on");
}

void EntropyControl::Manual() {
	BOXR_RUN_COMMANDF("/tie off");
	BOXR_RUN_COMMANDF("/timed 1 /home clear");
}

void EntropyControl::BurnNow() {
	BOXR_RUN_COMMANDF("/burn force on");
	LOG("Will burn all the time. Use \ay/burn force off\ax to stop burning.");
}

void EntropyControl::SetRaidAssistNum(int raidAssistNum) {
	BOXR_RUN_COMMANDF("/cc ass smart %d", raidAssistNum);
}

bool isClassPluginLoaded() {
	const char* classPluginName = getClassPlugin();
	if (classPluginName == nullptr) {
		return false;
	}
	return IsPluginLoaded(classPluginName);
}

const char* getClassPlugin() {
	switch (GetPcProfile()->Class) {
	case Bard:
		return "MQ2Bard";
	case Beastlord:
		return "MQ2Bst";
	case Berserker:
		return "MQ2BerZerker";
	case Cleric:
		return "MQ2Cleric";
	case Druid:
		return "MQ2Druid";
	case Enchanter:
		return "MQ2Enchanter";
	case Mage:
		return "MQ2Mage";
	case Monk:
		return "MQ2Monk";
	case Necromancer:
		return "MQ2Necro";
	case Paladin:
		return "MQ2Paladin";
	case Ranger:
		return "MQ2Ranger";
	case Rogue:
		return "MQ2Rogue";
	case Shadowknight:
		return "MQ2Eskay";
	case Shaman:
		return "MQ2Shaman";
	case Warrior:
		return "MQ2War";
	case Wizard:
		return "MQ2Wizard";
	default:
		return nullptr;
	}
}
