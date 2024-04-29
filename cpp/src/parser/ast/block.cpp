#include "block.h"

namespace interp::ast
{
	BlockExpression::BlockExpression(interp::token::Token token)
		: token(token), statements({})
	{
	}

	std::string BlockExpression::token_literal()
	{
		return this->token.literal;
	}

	std::string BlockExpression::string()
	{
		std::string out = "{ ";
		for (auto stmnt : this->statements)
		{
			out += stmnt->string() + " ";
		}
		out += "}";

		return out;
	}

	NodeType BlockExpression::type()
	{
		return NodeType::BlockExpression;
	}
}