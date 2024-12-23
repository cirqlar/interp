#pragma once

#include <vector>

#include "ast.h"
#include "base_obj.h"
#include "environment.h"

namespace interp::object
{
	class FunctionObject : public Object
	{
	public:
		FunctionObject(interp::ast::FunctionLiteral*, std::shared_ptr<Environment>);
		~FunctionObject() = default;

		std::vector<std::shared_ptr<interp::ast::Identifier>> params;
		std::shared_ptr<interp::ast::Expression> body;
		std::shared_ptr<Environment> environment;

		ObjectType type() const override;
		std::string inspect() const override;
	};
}