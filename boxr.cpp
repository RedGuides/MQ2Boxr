#include "boxr.h"

#include <fmt/format.h>
#include <string_view>

// Need to pace macro commands; the macro has to issue a /doevents between
// each command.
#define MACRO_COMMAND_DELAY "/timed 3 "

template <typename... Args>
void boxrRunCommandf(std::string_view format, Args&&... args) {
	auto command = fmt::format(format, std::forward<Args>(args)...);
	if (LOGGER.isDebugEnabled()) {
		DoCommandf(command.c_str());
		LOGGER.debug("Running Command: {}", command);
	}
	else {
		DoCommandf("/squelch %s", command.c_str());
	}
}

void MasterBoxControl::Pause() {
	auto box = getBox();
	LOGGER.info("Pausing {}", box->GetName());
	box->Pause();
}

void MasterBoxControl::Unpause() {
	auto box = getBox();
	LOGGER.info("Unpausing {}", box->GetName());
	box->Unpause();
}

void MasterBoxControl::Chase() {
	LOGGER.info("Setting \ayCHASE\ax mode");
	getBox()->Chase();
}

void MasterBoxControl::Camp() {
	LOGGER.info("Setting \ayCAMP\ax mode");
	getBox()->Camp();
}

void MasterBoxControl::Manual() {
	LOGGER.info("Setting \ayMANUAL\ax mode");
	getBox()->Manual();
}

void MasterBoxControl::BurnNow() {
	LOGGER.info("Burn phase NOW!");
	getBox()->BurnNow();
}

void MasterBoxControl::RaidAssistNum(int raidAssistNum) {
	LOGGER.info("Setting \a-tRaidAssistNum\ax to \at{}\ax (\at{}\ax)", raidAssistNum,
		GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
	LOGGER.debug("RaidAssist 1: {}", GetCharInfo()->raidData.MainAssistNames[0]);
	LOGGER.debug("RaidAssist 2: {}", GetCharInfo()->raidData.MainAssistNames[1]);
	LOGGER.debug("RaidAssist 3: {}", GetCharInfo()->raidData.MainAssistNames[2]);
	getBox()->SetRaidAssistNum(raidAssistNum);
}

MasterBoxControl::MasterBoxControl() {
	// Assume that if a macro is running, it is controlling the character, even if
	// a the class's CWTN plugin is loaded (since otherwise, why start the macro?)
	boxes.push_back(std::make_shared<RGMercsControl>());
	boxes.push_back(std::make_shared<KissAssistControl>());
	boxes.push_back(std::make_shared<MuleAssistControl>());
	boxes.push_back(std::make_shared<EntropyControl>());
	boxes.push_back(std::make_shared<AlsoKissAssistControl>());
	boxes.push_back(std::make_shared<CwtnControl>());
}

bool stringStartsWith(const char* pre, const char* str) {
	return _strnicmp(pre, str, strlen(pre)) == 0;
}

std::shared_ptr<BoxControl> MasterBoxControl::getBox() {
	for (std::shared_ptr<BoxControl> box : boxes) {
		if (box->isRunning()) {
			LOGGER.debug("Detected running: {}", box->GetName());
			return box;
		}
	}
	return this->noopControl;
}

void pauseTwist() {
	if (GetPcProfile()->Class == Bard) {
		boxrRunCommandf("/twist off");
	}
}

bool RGMercsControl::isRunning() {
	return stringStartsWith("rgmercs", gszMacroName);
}

void RGMercsControl::Pause() {
	boxrRunCommandf("/mqp on");
	pauseTwist();
}

void RGMercsControl::Unpause() {
	boxrRunCommandf("/mqp off");
}

void RGMercsControl::Chase() {
	boxrRunCommandf("/rg chaseon");
}

void RGMercsControl::Camp() {
	boxrRunCommandf("/rg camphard");
}

void RGMercsControl::Manual() {
	boxrRunCommandf("/rg chaseoff");
	boxrRunCommandf(MACRO_COMMAND_DELAY "/rg campoff");
}

void RGMercsControl::BurnNow() {
	LOGGER.info("BurnNow is not supported for rgmercs");
}

void RGMercsControl::SetRaidAssistNum(int raidAssistNum) {
	boxrRunCommandf("/rg AssistOutside 1");
	boxrRunCommandf(MACRO_COMMAND_DELAY "/rg OutsideAssistList {}", GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
}

bool KissAssistControl::isRunning() {
	return stringStartsWith("kiss", gszMacroName);
}

void KissAssistControl::Pause() {
	boxrRunCommandf("/mqp on");
	pauseTwist();
}

void KissAssistControl::Unpause() {
	boxrRunCommandf("/mqp off");
}

void KissAssistControl::Chase() {
	boxrRunCommandf("/chaseon");
}

void KissAssistControl::Camp() {
	boxrRunCommandf("/camphere on");
}

void KissAssistControl::Manual() {
	boxrRunCommandf("/chaseoff");
	boxrRunCommandf(MACRO_COMMAND_DELAY "/camphere off ");
}

void KissAssistControl::BurnNow() {
	boxrRunCommandf("/burn on doburn");
}

void KissAssistControl::SetRaidAssistNum(int raidAssistNum) {
	boxrRunCommandf("/switchma {} tank 1", GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
}

bool MuleAssistControl::isRunning() {
	return stringStartsWith("muleassist", gszMacroName);
}

void MuleAssistControl::BurnNow() {
	boxrRunCommandf("/burn");
}

void MuleAssistControl::SetRaidAssistNum(int raidAssistNum) {
	boxrRunCommandf("/changema {}", GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
}

bool AlsoKissAssistControl::isRunning() {
	return strstr(gszMacroName, "alsokissassist") != nullptr;
}

void AlsoKissAssistControl::SetRaidAssistNum(int raidAssistNum) {
	boxrRunCommandf("/switchma {}", GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]);
}

const char* getPlayerClassAbbr() {
	return ClassInfo[GetPcProfile()->Class].ShortName;
}

bool CwtnControl::isRunning() {
	return isClassPluginLoaded();
}

void CwtnControl::Pause() {
	boxrRunCommandf("/{} pause on", getPlayerClassAbbr());
}

void CwtnControl::Unpause() {
	boxrRunCommandf("/{} pause off", getPlayerClassAbbr());
}

void CwtnControl::Chase() {
	boxrRunCommandf("/{} mode chase", getPlayerClassAbbr());
}

void CwtnControl::Camp() {
	boxrRunCommandf("/{} mode assist", getPlayerClassAbbr());
	boxrRunCommandf("/{} resetcamp", getPlayerClassAbbr());
}

void CwtnControl::Manual() {
	boxrRunCommandf("/{} mode manual", getPlayerClassAbbr());
}

void CwtnControl::BurnNow() {
	boxrRunCommandf("/{} BurnNow", getPlayerClassAbbr());
}

void CwtnControl::SetRaidAssistNum(int raidAssistNum) {
	boxrRunCommandf("/{} raidassistnum {}", getPlayerClassAbbr(), raidAssistNum);
}

bool EntropyControl::isRunning() {
	return stringStartsWith("entropy", gszMacroName);
}

void EntropyControl::Pause() {
	boxrRunCommandf("/mqp on");
}

void EntropyControl::Unpause() {
	boxrRunCommandf("/mqp off");
}

void EntropyControl::Chase() {
	boxrRunCommandf("/tie on");
}

void EntropyControl::Camp() {
	boxrRunCommandf("/tie off");
	boxrRunCommandf(MACRO_COMMAND_DELAY "/home set on");
}

void EntropyControl::Manual() {
	boxrRunCommandf("/env auto off");
}

void EntropyControl::BurnNow() {
	boxrRunCommandf("/burn force on");
	LOGGER.info("Will burn all the time. Use \ay/burn force off\ax to stop burning.");
}

void EntropyControl::SetRaidAssistNum(int raidAssistNum) {
	boxrRunCommandf("/cc ass smart {}", raidAssistNum);
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
