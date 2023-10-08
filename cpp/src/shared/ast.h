#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "shared/token.h"

namespace interp::ast
{
	class Node
	{
	public:
		virtual std::string token_literal() = 0;
		virtual std::string string() = 0;
	};

	class Statement : public Node
	{
	};

	class Expression : public Node
	{
	};


	class Program: public Node
	{
	public:
		Program(std::vector<std::shared_ptr<Statement>> statements);
		Program() = default;
		~Program() = default;

		std::vector<std::shared_ptr<Statement>> statements;

		std::string token_literal() override;
		std::string string() override;
	};

	class Identifier: public Expression
	{
	public:
		Identifier(interp::token::Token token, std::string value);
		~Identifier() = default;

		interp::token::Token token;
		std::string value;

		std::string token_literal() override;
		std::string string() override;
	};

	class LetStatement: public Statement
	{
	public:
		LetStatement(interp::token::Token token, Identifier name, std::shared_ptr<Expression> value);
		~LetStatement() = default;

		interp::token::Token token;
		Identifier name;
		std::shared_ptr<Expression> value;

		std::string token_literal() override;
		std::string string() override;
	};

	class ReturnStatement: public Statement
	{
	public:
		ReturnStatement(interp::token::Token token, std::shared_ptr<Expression> return_value);
		~ReturnStatement() = default;

		interp::token::Token token;
		std::shared_ptr<Expression> return_value;

		std::string token_literal() override;
		std::string string() override;
	};

	class ExpressionStatement: public Statement
	{
	public:
		interp::token::Token token;
		std::unique_ptr<Expression> expression;

		std::string token_literal() override;
		std::string string() override;
	};

	

}
