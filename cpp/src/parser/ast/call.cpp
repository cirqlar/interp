#include "call.h"

interp::ast::CallExpression::CallExpression(interp::token::Token token)
	: token(token), function(nullptr), args({})
{
}

interp::ast::CallExpression::CallExpression(interp::token::Token token, std::shared_ptr<interp::ast::Expression> function)
	: token(token), function(function), args({})
{
}

std::string interp::ast::CallExpression::token_literal()
{
	return this->token.literal;
}

std::string interp::ast::CallExpression::string()
{
	std::string out = this->function->string() + "(";

	for (size_t i = 0; i < this->args.size(); i++)
	{
		out += this->args[i]->string();
		if (i < this->args.size() - 1)
		{
			out += ", ";
		}
	}

	out += ")";

	return out;
}
