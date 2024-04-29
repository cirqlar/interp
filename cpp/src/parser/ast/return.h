#pragma once

#include "node.h"
#include "token.h"

namespace interp::ast
{
	class ReturnStatement : public Statement
	{
	public:
		ReturnStatement(interp::token::Token token, std::shared_ptr<Expression> return_value);
		~ReturnStatement() = default;

		interp::token::Token token;
		std::shared_ptr<Expression> return_value;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() override;
	};
}