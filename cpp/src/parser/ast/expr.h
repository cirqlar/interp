#pragma once

#include "node.h"
#include "token.h"

namespace interp::ast
{
	class ExpressionStatement : public Statement
	{
	public:
		ExpressionStatement(interp::token::Token token, std::shared_ptr<Expression> expression);
		~ExpressionStatement() = default;

		interp::token::Token token;
		std::shared_ptr<Expression> expression;

		std::string token_literal() override;
		std::string string() override;
	};
}