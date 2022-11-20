#include "boxr_util.h"
#include "boxr_logger.h"
#include <mq/Plugin.h>

bool EvaluateBooleanMacroExpression(const std::string& expression) {
	CHAR evaluatedExpression[MAX_STRING];
	strcpy_s(evaluatedExpression, expression.c_str());
	ParseMacroData(evaluatedExpression, MAX_STRING);
	double CalculateResult = 0;
	Calculate(evaluatedExpression, CalculateResult);
	return CalculateResult != 0;
}
