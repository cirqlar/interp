#pragma once

#include "node.h"
#include "token.h"

namespace interp::ast
{
	class PrefixExpression : public Expression
	{
	public:
		PrefixExpression(interp::token::Token token, std::string p_operator, std::shared_ptr<Expression> right);
		~PrefixExpression() = default;

		interp::token::Token token;
		std::string p_operator;
		std::shared_ptr<Expression> right;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() const override;
	};
}