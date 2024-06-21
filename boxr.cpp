#include <fmt/format.h>

#include "boxr.h"

#include "boxr_util.h"
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
	} else {
		DoCommandf("/squelch %s", command.c_str());
	}
}

void MasterBoxControl::Pause() {
	auto box = GetBox();
	LOGGER.info("Pausing {}", box->GetName());
	box->Pause();
}

void MasterBoxControl::Unpause() {
	auto box = GetBox();
	LOGGER.info("Unpausing {}", box->GetName());
	box->Unpause();
}

void MasterBoxControl::Chase() {
	LOGGER.info("Setting \ayCHASE\ax mode");
	GetBox()->Chase();
}

void MasterBoxControl::Camp() {
	LOGGER.info("Setting \ayCAMP\ax mode");
	GetBox()->Camp();
}

void MasterBoxControl::Manual() {
	LOGGER.info("Setting \ayMANUAL\ax mode");
	GetBox()->Manual();
}

void MasterBoxControl::BurnNow() {
	LOGGER.info("Burn phase NOW!");
	GetBox()->BurnNow();
}

void MasterBoxControl::RaidAssistNum(int raidAssistNum) {
#if !defined(ROF2EMU) && !defined(UFEMU)
	LOGGER.info("Setting \a-tRaidAssistNum\ax to \at{}\ax (\at{}\ax)", raidAssistNum, GetRaidMainAssistName(raidAssistNum));
	LOGGER.debug("RaidAssist 1: {}", GetCharInfo()->raidData.MainAssistNames[0]);
	LOGGER.debug("RaidAssist 2: {}", GetCharInfo()->raidData.MainAssistNames[1]);
	LOGGER.debug("RaidAssist 3: {}", GetCharInfo()->raidData.MainAssistNames[2]);
	GetBox()->SetRaidAssistNum(raidAssistNum);
#endif
}

bool MasterBoxControl::IsPaused() {
	auto box = GetBox();
	try {
		LOGGER.debug("Checking if \aw{}\ax is paused by evaluating '\ay{}\ax'", box->GetName(), box->GetPauseQuery());
		return EvaluateBooleanMacroExpression(box->GetPauseQuery());
	} catch (std::runtime_error &e) {
		throw InvalidBoxConfigurationException(fmt::format("Unable to determine whether \aw{}\ax is paused: {}", box->GetName(), e.what()));
	}
}

std::string MasterBoxControl::Current() {
	return GetBox()->GetKey();
}

MasterBoxControl::MasterBoxControl() {
	// Assume that if a macro is running, it is controlling the character, even if
	// a the class's CWTN plugin is loaded (since otherwise, why start the macro?)
	boxes.push_back(std::make_shared<RGMercsControl>());
	boxes.push_back(std::make_shared<KissAssistControl>());
	boxes.push_back(std::make_shared<MuleAssistControl>());
	boxes.push_back(std::make_shared<EntropyControl>());
	boxes.push_back(std::make_shared<AlsoKissAssistControl>());
	boxes.push_back(std::make_shared<XGenControl>());
	boxes.push_back(std::make_shared<RGMercsLuaControl>());
	boxes.push_back(std::make_shared<CwtnControl>());
}

std::shared_ptr<BoxControl> MasterBoxControl::GetBox() {
	for (std::shared_ptr<BoxControl> box : boxes) {
		if (box->IsRunning()) {
			LOGGER.debug("Detected running: {}", box->GetName());
			return box;
		}
	}
	return this->noopControl;
}

void PauseTwist() {
	if (GetPcProfile()->Class == Bard) {
		boxrRunCommandf("/twist off");
	}
}

bool RGMercsControl::IsRunning() {
	return ci_starts_with(gszMacroName, "rgmercs");
}

void RGMercsControl::Pause() {
	boxrRunCommandf("/mqp on");
	PauseTwist();
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
	boxrRunCommandf(MACRO_COMMAND_DELAY "/rg OutsideAssistList {}", GetRaidMainAssistName(raidAssistNum));
}

bool RGMercsLuaControl::IsRunning() {
	return EvaluateBooleanMacroExpression("${Lua.Script[rgmercs].Status.Equal[RUNNING]}");
}

void RGMercsLuaControl::Pause() {
	boxrRunCommandf("/rgl pause");
	PauseTwist();
}

void RGMercsLuaControl::Unpause() {
	boxrRunCommandf("/rgl unpause");
}

void RGMercsLuaControl::Chase() {
	boxrRunCommandf("/rgl chaseon");
}

void RGMercsLuaControl::Camp() {
	boxrRunCommandf("/rgl campon");
}

void RGMercsLuaControl::Manual() {
	boxrRunCommandf("/rgl chaseoff");
	boxrRunCommandf("/rgl campoff");
}

void RGMercsLuaControl::BurnNow() {
	LOGGER.info("BurnNow is not supported for RGMercs - Lua edition");
}

void RGMercsLuaControl::SetRaidAssistNum(int raidAssistNum) {
	LOGGER.info("RaidAssistNum is not supported for RGMercs - Lua edition");
}

bool KissAssistControl::IsRunning() {
	return ci_starts_with(gszMacroName, "kiss");
}

void KissAssistControl::Pause() {
	boxrRunCommandf("/mqp on");
	PauseTwist();
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
	boxrRunCommandf("/switchma {} tank 1", GetRaidMainAssistName(raidAssistNum));
}

bool MuleAssistControl::IsRunning() {
	return ci_starts_with(gszMacroName, "muleassist");
}

void MuleAssistControl::BurnNow() {
	boxrRunCommandf("/burn");
}

void MuleAssistControl::SetRaidAssistNum(int raidAssistNum) {
	boxrRunCommandf("/changema {}", GetRaidMainAssistName(raidAssistNum));
}

bool AlsoKissAssistControl::IsRunning() {
	return strstr(gszMacroName, "alsokissassist") != nullptr;
}

void AlsoKissAssistControl::SetRaidAssistNum(int raidAssistNum) {
	boxrRunCommandf("/switchma {}", GetRaidMainAssistName(raidAssistNum));
}

bool CwtnControl::IsRunning() {
	return IsClassPluginLoaded();
}

void CwtnControl::Pause() {
	boxrRunCommandf("/{} pause on", GetClassCommand());
}

void CwtnControl::Unpause() {
	boxrRunCommandf("/{} pause off", GetClassCommand());
}

void CwtnControl::Chase() {
	boxrRunCommandf("/{} mode chase", GetClassCommand());
}

void CwtnControl::Camp() {
	boxrRunCommandf("/{} mode assist", GetClassCommand());
	boxrRunCommandf("/{} resetcamp", GetClassCommand());
}

void CwtnControl::Manual() {
	boxrRunCommandf("/{} mode manual", GetClassCommand());
}

void CwtnControl::BurnNow() {
	boxrRunCommandf("/{} BurnNow", GetClassCommand());
}

void CwtnControl::SetRaidAssistNum(int raidAssistNum) {
	boxrRunCommandf("/{} raidassistnum {}", GetClassCommand(), raidAssistNum);
}

bool EntropyControl::IsRunning() {
	return ci_starts_with(gszMacroName, "entropy");
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

bool XGenControl::IsRunning() {
	return ci_starts_with(gszMacroName, "xgen");
}

void XGenControl::Pause() {
	boxrRunCommandf("/mqp on");
}

void XGenControl::Unpause() {
	boxrRunCommandf("/mqp off");
}

void XGenControl::Chase() {
	boxrRunCommandf("/cc follow");
	boxrRunCommandf(MACRO_COMMAND_DELAY "/cc camp off");
}

void XGenControl::Camp() {
	boxrRunCommandf("/cc camp on");
	boxrRunCommandf(MACRO_COMMAND_DELAY "/cc follow off");
}

void XGenControl::Manual() {
	boxrRunCommandf("/cc manual");
}

void XGenControl::BurnNow() {
	boxrRunCommandf("/cc burnonce");
	LOGGER.info("Will burn current mob only.");
}

void XGenControl::SetRaidAssistNum(int raidAssistNum) {
	boxrRunCommandf("/cc setassist {}", raidAssistNum);
}

const char* CwtnControl::GetClassCommand() {
	return ClassInfo[GetPcProfile()->Class].ShortName;
}

bool CwtnControl::IsClassPluginLoaded() {
	const char* classPluginName = GetClassPlugin();
	if (classPluginName == nullptr) {
		return false;
	}
	return IsPluginLoaded(classPluginName);
}

const char* CwtnControl::GetClassPlugin() {
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
