#pragma once

#include "node.h"
#include "lexer/token.h"

namespace interp::ast
{
	class IfExpression : public Expression
	{
	public:
		IfExpression(interp::token::Token token, std::shared_ptr<Expression> condition, std::shared_ptr<Expression> consequence, std::shared_ptr<Expression> alternative = nullptr);
		~IfExpression() = default;

		interp::token::Token token;
		std::shared_ptr<Expression> condition;
		std::shared_ptr<Expression> consequence;
		std::shared_ptr<Expression> alternative;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() const override;
	};
}