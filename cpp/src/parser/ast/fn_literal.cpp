#include "fn_literal.h"

interp::ast::FunctionLiteral::FunctionLiteral(interp::token::Token token)
	: token(token), params({}), body(nullptr)
{
}

std::string interp::ast::FunctionLiteral::token_literal()
{
	return this->token.literal;
}

std::string interp::ast::FunctionLiteral::string()
{
	std::string out = "";

	out += this->token.literal;
	out += "(";

	for (size_t i = 0; i < this->params.size(); i++)
	{
		out += this->params[i]->value;
		if (i < this->params.size() - 1)
		{
			out += ", ";
		}
	}

	out += ") ";
	out += this->body->string();


	return out;
}
