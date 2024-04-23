#pragma once

#include "node.h"
#include "token.h"

namespace interp::ast
{
	class Boolean : public Expression
	{
	public:
		Boolean(interp::token::Token token, bool value);
		~Boolean() = default;

		interp::token::Token token;
		bool value;

		std::string token_literal() override;
		std::string string() override;
	};
}