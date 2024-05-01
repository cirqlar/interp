#pragma once

#include "base_obj.h"

namespace interp::object
{
	class ErrorObject : public Object
	{
	public:
		ErrorObject(std::string message);
		~ErrorObject() = default;

		std::string message;

		ObjectType type() const override;
		std::string inspect() const override;
	};
}