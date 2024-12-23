#pragma once

#include "node.h"
#include "lexer/token.h"

namespace interp::ast
{
	class Identifier : public Expression
	{
	public:
		Identifier(interp::token::Token token, std::string value);
		~Identifier() = default;

		interp::token::Token token;
		std::string value;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() const override;
	};
}