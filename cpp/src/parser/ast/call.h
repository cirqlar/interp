#pragma once

#include <vector>

#include "node.h"
#include "token.h"

namespace interp::ast
{
	class CallExpression : public Expression
	{
	public:
		CallExpression(interp::token::Token token);
		CallExpression(interp::token::Token token, std::shared_ptr<Expression> function);
		~CallExpression() = default;

		interp::token::Token token;
		std::shared_ptr<Expression> function;
		std::vector<std::shared_ptr<Expression>> args;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() override;
	};
}