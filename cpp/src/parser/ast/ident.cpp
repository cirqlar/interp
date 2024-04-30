#include "ident.h"

namespace interp::ast
{
	Identifier::Identifier(interp::token::Token token, std::string value) : token(token), value(value)
	{
	}

	std::string Identifier::token_literal()
	{
		return this->token.literal;
	}

	std::string Identifier::string()
	{
		return this->value;
	}

	NodeType Identifier::type() const
	{
		return NodeType::Identifier;
	}
}