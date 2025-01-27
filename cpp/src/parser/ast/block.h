#pragma once

#include <vector>

#include "node.h"
#include "lexer/token.h"

namespace interp::ast
{
	class BlockExpression : public Expression
	{
	public:
		BlockExpression(interp::token::Token token);
		~BlockExpression() = default;

		interp::token::Token token;
		std::vector<std::shared_ptr<Statement>> statements;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() const override;
	};
}