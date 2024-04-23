#include "let.h"

namespace interp::ast
{
	LetStatement::LetStatement(interp::token::Token token, Identifier name, std::shared_ptr<Expression> value)
		: token(token), name(name), value(value)
	{
	}

	std::string LetStatement::token_literal()
	{
		return this->token.literal;
	}

	std::string LetStatement::string()
	{
		std::string out = this->token_literal() + " " + this->name.string() + " = ";

		if (this->value)
		{
			out += this->value->string();
		}

		out += ";";
		return out;
	}
}