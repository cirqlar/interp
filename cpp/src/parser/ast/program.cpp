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
		for (size_t i = 0; i < this->statements.size(); i++)
		{
			out += this->statements[i]->string();

			if (i < this->statements.size() - 1)
			{
				out += '\n';
			}
		}

		return out;
	}
}