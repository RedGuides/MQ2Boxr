#include "boxr.h"
#include "boxr_util.h"
#include "boxr_logger.h"
#include <mq/Plugin.h>

bool EvaluateBooleanMacroExpression(const std::string& expression) {
	CHAR evaluatedExpression[MAX_STRING];
	strcpy_s(evaluatedExpression, expression.c_str());
	ParseMacroData(evaluatedExpression, MAX_STRING);
	double calculateResult = 0;
	Calculate(evaluatedExpression, calculateResult);
	return calculateResult != 0;
}

char* GetRaidMainAssistName(int raidAssistNum) {
#if defined(ROF2EMU) || defined(UFEMU)
	throw UnsupportedBoxrOperationException("Boxr does not support Raid MA operations on EMU");
#else
	return GetCharInfo()->raidData.MainAssistNames[raidAssistNum - 1];
#endif
}
