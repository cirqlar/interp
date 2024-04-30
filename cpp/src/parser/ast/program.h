#pragma once

#include <vector>

#include "node.h"
#include "token.h"

namespace interp::ast
{
	class Program : public Node
	{
	public:
		Program(std::vector<std::shared_ptr<Statement>> statements);
		Program() = default;
		~Program() = default;

		std::vector<std::shared_ptr<Statement>> statements;

		std::string token_literal() override;
		std::string string() override;
		NodeType type() const override;
	};
}