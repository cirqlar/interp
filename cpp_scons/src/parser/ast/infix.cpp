#include "infix.h"

namespace interp::ast
{
	InfixExpression::InfixExpression(interp::token::Token token, std::shared_ptr<Expression> left, std::string p_operator, std::shared_ptr<Expression> right)
		: token(token), left(left), p_operator(p_operator), right(right)
	{
	}

	std::string InfixExpression::token_literal()
	{
		return this->token.literal;
	}

	std::string InfixExpression::string()
	{
		return "(" + this->left->string() + " " + this->token_literal() + " " + this->right->string() + ")";
	}

	NodeType InfixExpression::type() const
	{
		return NodeType::InfixExpression;
	}
}