#pragma once

#include "node.h"
#include "lexer/token.h"

namespace interp::ast
{
	class BooleanLiteral : public Expression
	{
	public:
		BooleanLiteral(interp::token::Token token, bool value);
		~BooleanLiteral() = default;

		interp::token::Token token;
		bool value;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() const override;
	};
}