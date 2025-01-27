#pragma once

#include "node.h"
#include "lexer/token.h"

namespace interp::ast
{
	class StringLiteral : public Expression
	{
	public:
		StringLiteral(interp::token::Token token, std::string value);
		~StringLiteral() = default;

		interp::token::Token token;
		std::string value;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() const override;
	};
}