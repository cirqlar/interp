#include "int.h"

namespace interp::ast
{
	IntegerLiteral::IntegerLiteral(interp::token::Token token, int64_t value) : token(token), value(value)
	{
	}

	std::string IntegerLiteral::token_literal()
	{
		return this->token.literal;
	}

	std::string IntegerLiteral::string()
	{
		return this->token.literal;
	}
}