#pragma once

#include "node.h"
#include "token.h"

namespace interp::ast
{
	class IntegerLiteral : public Expression
	{
	public:
		IntegerLiteral(interp::token::Token token, int64_t value);
		~IntegerLiteral() = default;

		interp::token::Token token;
		int64_t value;

		std::string token_literal() override;
		std::string string() override;
	};
}