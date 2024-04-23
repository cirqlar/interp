#include "bool.h"

namespace interp::ast
{
	Boolean::Boolean(interp::token::Token token, bool value)
		: token(token), value(value)
	{
	}

	std::string Boolean::token_literal()
	{
		return this->token.literal;
	}

	std::string Boolean::string()
	{
		return this->token.literal;
	}
}