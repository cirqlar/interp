#include "func_obj.h"

namespace interp::object
{
	FunctionObject::FunctionObject(interp::ast::FunctionLiteral* fn_lit, std::shared_ptr<Environment> environment)
	{
		this->params = fn_lit->params;
		this->body = fn_lit->body;
		this->environment = environment;
	}

	ObjectType FunctionObject::type() const
	{
		return ObjectType::FunctionObject;
	}

	std::string FunctionObject::inspect() const
	{
		std::string out = "";

		out += "fn";
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
}