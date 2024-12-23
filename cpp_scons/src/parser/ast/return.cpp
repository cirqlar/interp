#include "return.h"

namespace interp::ast
{
	ReturnStatement::ReturnStatement(interp::token::Token token, std::shared_ptr<Expression> return_value)
		: token(token), return_value(return_value)
	{
	}

	std::string ReturnStatement::token_literal()
	{
		return this->token.literal;
	}

	std::string ReturnStatement::string()
	{
		std::string out = this->token_literal() + " ";
		if (this->return_value)
		{
			out += this->return_value->string();
		}
		out += ";";
		return out;
	}

	NodeType ReturnStatement::type() const
	{
		return NodeType::ReturnStatment;
	}
}