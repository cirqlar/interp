#pragma once

#include "node.h"
#include "token.h"

namespace interp::ast
{
	class InfixExpression : public Expression
	{
	public:
		InfixExpression(interp::token::Token token, std::shared_ptr<Expression> left, std::string p_operator, std::shared_ptr<Expression> right);
		~InfixExpression() = default;

		interp::token::Token token;
		std::shared_ptr<Expression> left;
		std::string p_operator;
		std::shared_ptr<Expression> right;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() override;
	};
}