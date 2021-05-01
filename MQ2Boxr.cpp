#include "../MQ2Plugin.h"

PLUGIN_VERSION(0.1);
PreSetup("MQ2Boxr");

#include "boxr.h"

std::vector<std::string> getArgVector(const std::string& argsString, const std::string& delimiter = " ") {
	auto args = std::vector<std::string>();
	size_t nextDelimiterStartPos;
	int nextTokenStartPos = 0;
	while ((nextDelimiterStartPos = argsString.find(delimiter, nextTokenStartPos)) != std::string::npos) {
		size_t tokenLength = nextDelimiterStartPos - nextTokenStartPos;
		args.push_back(argsString.substr(nextTokenStartPos, tokenLength));
		nextTokenStartPos = nextDelimiterStartPos + delimiter.length();
	}
	args.push_back(argsString.substr(nextTokenStartPos));
	return args;
}

bool iStrEquals(const char* arg1, const std::string& arg2) {
	return _strcmpi(arg1, arg2.c_str()) == 0;
}

bool parseBoolArg(std::string arg) {
	if (iStrEquals("on", arg) || iStrEquals("true", arg) || iStrEquals("yes", arg) || iStrEquals("y", arg) ||
		iStrEquals("1", arg)) {
		return true;
	}
	else if (iStrEquals("off", arg) || iStrEquals("false", arg) || iStrEquals("no", arg) || iStrEquals("n", arg) ||
		iStrEquals("0", arg)) {
		return false;
	}
	throw std::invalid_argument("Invalid argument. Valid values are: [on, off, true, false, yes, no, 1, 0]");
}

void printUsage() {
	LOG("Boxr commands:\n"
		"\a-t|\ax  \ay/boxr Pause\ax - Pauses the character\n"
		"\a-t|\ax  \ay/boxr Unpause\ax - Unpause the character\n"
		"\a-t|\ax  \ay/boxr Chase\ax - Sets navivation to chase assisted character, and stop camping\n"
		"\a-t|\ax  \ay/boxr Camp\ax - Sets navigation to camp at current position, and return to camp after combat\n"
		"\a-t|\ax  \ay/boxr Manual\ax - Sets manual navigation (don't chase, no camp)\n"
		"\a-t|\ax  \ay/boxr RaidAssistNum <1, 2, 3>\ax - Toggles which Raid MA to assist\n"
		"\a-t|\ax  \ay/boxr Debug \a-y[on|off]\ax\ax - Toggles MQ2Boxr debug logging\n"
		"\a-t|\ax  \ay/boxr Help\ax - Prints this help\n"
	);
}

void BoxrCommand(SPAWNINFO* pChar, char* szLine) {
	if (GetGameState() != GAMESTATE_INGAME) {
		LOG_ERROR("Was asked to do '%s', but am not in game");
		return;
	}

	auto argVector = getArgVector(szLine);

	if (argVector.empty() || iStrEquals("help", argVector.front())) {
		printUsage();
	} else if (iStrEquals("pause", argVector.front())) {
		MasterBoxControl::getInstance().Pause();
	} else if (iStrEquals("unpause", argVector.front())) {
		MasterBoxControl::getInstance().Unpause();
	} else if (iStrEquals("chase", argVector.front())) {
		MasterBoxControl::getInstance().Chase();
	} else if (iStrEquals("camp", argVector.front())) {
		MasterBoxControl::getInstance().Camp();
	} else if (iStrEquals("manual", argVector.front())) {
		MasterBoxControl::getInstance().Manual();
	} else if (iStrEquals("raidassistnum", argVector.front())) {
		if (argVector.size() != 2) {
			LOG_ERROR("/boxr RaidAssistNum: expected exactly one argument, but got %d", argVector.size() - 1);
			return;
		}
		try {
			int raidAssistNum = std::stoi(argVector.at(1));
			if (raidAssistNum < 1 || raidAssistNum > 3) {
				LOG_ERROR("/boxr RaidAssistNum: RaidAssistNum must be either 1, 2, or 3.");
				return;
			}
#if defined(ROF2EMU) || defined(UFEMU)
			LOG_ERROR("/boxr RaidAssistNum: There is no main assist %d on EMU", raidAssistNum);
			return;
#else
			if (!GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1]) {
				LOG_ERROR("/boxr RaidAssistNum: There is no main assist %d", raidAssistNum);
				return;
			}
#endif
			MasterBoxControl::getInstance().RaidAssistNum(raidAssistNum);
		} catch (std::invalid_argument& e) {
			LOG_ERROR("/boxr raidassistnum: invalid argument - expected 1, 2, or 3, but got: %s", argVector.at(1).c_str());
		}
	} else if (iStrEquals("debug", argVector.front())) {
		try {
			debugEnabled = (argVector.size() == 1)
				? !debugEnabled
				: parseBoolArg(argVector.at(1));
			LOG("Debug logging %s", debugEnabled ? "enabled" : "disabled");
		} catch (std::invalid_argument& e) {
			LOG_ERROR("/boxr debug: %s", e.what());
		}
	} else {
		LOG("Do not understand command: %s", szLine);
	}
}

// Called once, when the plugin is to initialize
PLUGIN_API VOID InitializePlugin(VOID) {
	DebugSpewAlways("Initializing MQ2Boxr");
	AddCommand("/boxr", BoxrCommand);
}

// Called once, when the plugin is to shutdown
PLUGIN_API VOID ShutdownPlugin(VOID) {
	DebugSpewAlways("Shutting down MQ2Boxr");
	RemoveCommand("/boxr");
}
