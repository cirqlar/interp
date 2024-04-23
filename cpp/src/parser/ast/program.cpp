#include "program.h"

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
}