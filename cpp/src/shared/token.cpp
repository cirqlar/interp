#include "token.h"

namespace interp::shared {
	std::map<std::string, TokenType> keywords({
		std::pair("fn", FUNCTION),
		std::pair("let", LET),
		std::pair("true", TRUE),
		std::pair("false", FALSE),
		std::pair("if", IF),
		std::pair("else", ELSE),
		std::pair("return", RETURN),
	});

	std::string lookup_ident(std::string literal)
	{
		if (keywords.find(literal) == keywords.end()) {
			return IDENT;
		} else {
			return keywords[literal];
		}
	};
}