#include "boxr_util.h"
#include "boxr_logger.h"
#include <mq/Plugin.h>

bool StringStartsWith(const char* pre, const char* str) {
	return _strnicmp(pre, str, strlen(pre)) == 0;
}

bool EvaluateBooleanMacroExpression(const std::string& expression) {
	CHAR evaluatedExpression[MAX_STRING];
	strcpy_s(evaluatedExpression, expression.c_str());
	ParseMacroData(evaluatedExpression, MAX_STRING);
	if (strcmp(evaluatedExpression, "NULL") == 0 || (strcmp(evaluatedExpression, "TRUE") != 0 && (strcmp(evaluatedExpression, "FALSE")))) {
		throw std::runtime_error(fmt::format("Expected \ay{}\ax to evaluate to either \ayTRUE\ax or \ayFALSE\ax, but got \ay{}\ax", expression, evaluatedExpression));
	}
	LOGGER.debug("%s evaluates to: %s", expression.c_str(), evaluatedExpression);
	return strcmp(evaluatedExpression, "TRUE") == 0;
}
