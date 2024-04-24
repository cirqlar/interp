#pragma once

#include <vector>

#include "ident.h"
#include "node.h"
#include "token.h"

namespace interp::ast
{
	class FunctionLiteral : public Expression
	{
	public:
		FunctionLiteral(interp::token::Token token);
		~FunctionLiteral() = default;

		interp::token::Token token;
		std::vector<std::shared_ptr<interp::ast::Identifier>> params;
		std::shared_ptr<interp::ast::Expression> body;

		std::string token_literal() override;
		std::string string() override;
	};
}