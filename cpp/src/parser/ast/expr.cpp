#include "expr.h"

namespace interp::ast
{
	ExpressionStatement::ExpressionStatement(interp::token::Token token, std::shared_ptr<Expression> expression)
		: token(token), expression(expression)
	{
	}

	std::string ExpressionStatement::token_literal()
	{
		return this->token.literal;
	}

	std::string ExpressionStatement::string()
	{
		if (this->expression)
		{
			return this->expression->string();
		}
		else
		{
			return "";
		}
	}

	NodeType ExpressionStatement::type()
	{
		return NodeType::ExpressionStatment;
	}
}