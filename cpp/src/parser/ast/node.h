#pragma once

#include <iostream>

namespace interp::ast
{

	class Node
	{
	public:
		virtual ~Node() = default;

		virtual std::string token_literal() = 0;
		virtual std::string string() = 0;
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