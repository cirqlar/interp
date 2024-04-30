#pragma once

#include <iostream>

namespace interp::ast
{
	enum struct NodeType
	{
		Program,
		BlockExpression,
		BooleanExpression,
		CallExpression,
		ExpressionStatment,
		FunctionLiteral,
		Identifier,
		IfExpression,
		InfixExpression,
		IntegerLiteral,
		LetStatment,
		PrefixExpression,
		ReturnStatment,
	};

	std::string node_type_to_string(NodeType node_type);

	class Node
	{
	public:
		virtual ~Node() = default;

		virtual std::string token_literal() = 0;
		virtual std::string string() = 0;
		virtual NodeType type() const = 0;
	};

	class Expression : public Node
	{
	public:
		virtual ~Expression() = default;
	};

	class Statement : public Node
	{
	public:
		virtual ~Statement() = default;
	};
}