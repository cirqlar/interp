#include "fn_literal.h"

namespace interp::ast
{
	FunctionLiteral::FunctionLiteral(interp::token::Token token)
		: token(token), params({}), body(nullptr)
	{
	}

	std::string FunctionLiteral::token_literal()
	{
		return this->token.literal;
	}

	std::string FunctionLiteral::string()
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

	NodeType FunctionLiteral::type()
	{
		return NodeType::FunctionLiteral;
	}
}

