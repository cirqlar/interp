#include "ast_string.h"

namespace interp::ast
{
	StringLiteral::StringLiteral(interp::token::Token token, std::string value) : token(token), value(value)
	{
	}

	std::string StringLiteral::token_literal()
	{
		return this->token.literal;
	}

	std::string StringLiteral::string()
	{
		return this->token.literal;
	}

	NodeType StringLiteral::type() const
	{
		return NodeType::StringLiteral;
	}
}