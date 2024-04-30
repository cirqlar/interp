#pragma once

#include "ident.h"
#include "node.h"
#include "token.h"

namespace interp::ast
{
	class LetStatement : public Statement
	{
	public:
		LetStatement(interp::token::Token token, Identifier name, std::shared_ptr<Expression> value);
		~LetStatement() = default;

		interp::token::Token token;
		Identifier name;
		std::shared_ptr<Expression> value;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() const override;
	};
}