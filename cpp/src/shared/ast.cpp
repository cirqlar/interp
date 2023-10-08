#include "ast.h"

namespace interp::ast
{
	Program::Program(std::vector<std::shared_ptr<Statement>> statements) : statements(statements)
	{
	}

	std::string Program::token_literal()
	{
		if (this->statements.size() > 0)
		{
			return this->statements.at(0)->token_literal();
		}
		else
		{
			return "";
		}
	}

	std::string Program::string()
	{
		std::string out;
		for (auto stmnt : this->statements)
		{
			out += stmnt->string();
		}

		return out;
	}

	Identifier::Identifier(interp::token::Token token, std::string value) : token(token), value(value)
	{
	}

	std::string Identifier::token_literal()
	{
		return this->token.literal;
	}

	std::string Identifier::string()
	{
		return this->value;
	}

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
}