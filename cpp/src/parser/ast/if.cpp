#include "if.h"

namespace interp::ast
{
	IfExpression::IfExpression(interp::token::Token token, std::shared_ptr<Expression> condition, std::shared_ptr<Expression> consequence, std::shared_ptr<Expression> alternative)
		: token(token), condition(condition), consequence(consequence), alternative(alternative)
	{
	}

	std::string IfExpression::token_literal()
	{
		return this->token.literal;
	}

	std::string IfExpression::string()
	{
		std::string out = "if " + this->condition->string() + " " + this->consequence->string();

		if (this->alternative)
		{
			out += "else " + this->alternative->string();
		}

		return out;
	}

	NodeType IfExpression::type()
	{
		return NodeType::IfExpression;
	}
}