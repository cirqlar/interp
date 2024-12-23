#include "bool.h"

namespace interp::ast
{
	BooleanLiteral::BooleanLiteral(interp::token::Token token, bool value)
		: token(token), value(value)
	{
	}

	std::string BooleanLiteral::token_literal()
	{
		return this->token.literal;
	}

	std::string BooleanLiteral::string()
	{
		return this->token.literal;
	}

	NodeType BooleanLiteral::type() const
	{
		return NodeType::BooleanExpression;
	}
}