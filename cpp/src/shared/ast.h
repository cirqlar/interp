#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <stdint.h>

#include "shared/token.h"

namespace interp::ast
{
	class Node
	{
	public:
		virtual ~Node() = default;

		virtual std::string token_literal() = 0;
		virtual std::string string() = 0;
	};

	// ******** Expressions ********
	class Expression : public Node
	{
	public:
		virtual ~Expression() = default;
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

	class IntegerLiteral: public Expression
	{
	public:
		IntegerLiteral(interp::token::Token token, int64_t value);
		~IntegerLiteral() = default;

		interp::token::Token token;
		int64_t value;

		std::string token_literal() override;
		std::string string() override;
	};

	class Boolean : public Expression
	{
	public:
		Boolean(interp::token::Token token, bool value);
		~Boolean() = default;

		interp::token::Token token;
		bool value;

		std::string token_literal() override;
		std::string string() override;
	};

	class PrefixExpression: public Expression
	{
	public:
		PrefixExpression(interp::token::Token token, std::string p_operator, std::shared_ptr<Expression> right);
		~PrefixExpression() = default;

		interp::token::Token token;
		std::string p_operator;
		std::shared_ptr<Expression> right;

		std::string token_literal() override;
		std::string string() override;
	};

	class InfixExpression : public Expression
	{
	public:
		InfixExpression(interp::token::Token token, std::shared_ptr<Expression> left, std::string p_operator, std::shared_ptr<Expression> right);
		~InfixExpression() = default;

		interp::token::Token token;
		std::shared_ptr<Expression> left;
		std::string p_operator;
		std::shared_ptr<Expression> right;

		std::string token_literal() override;
		std::string string() override;
	};

	// ******** Statements ********
	class Statement : public Node
	{
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
		ExpressionStatement(interp::token::Token token, std::shared_ptr<Expression> expression);
		~ExpressionStatement() = default;

		interp::token::Token token;
		std::shared_ptr<Expression> expression;

		std::string token_literal() override;
		std::string string() override;
	};

	// ******** Program ********
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
	

}
