#include "prefix.h"

namespace interp::ast
{
	PrefixExpression::PrefixExpression(interp::token::Token token, std::string p_operator, std::shared_ptr<Expression> right)
		: token(token), p_operator(p_operator), right(right)
	{
	}

	std::string PrefixExpression::token_literal()
	{
		return this->token.literal;
	}

	std::string PrefixExpression::string()
	{
		return "(" + this->token_literal() + this->right->string() + ")";
	}

	NodeType PrefixExpression::type()
	{
		return NodeType::PrefixExpression;
	}
}