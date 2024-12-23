#pragma once

#include "base_obj.h"

namespace interp::object
{
	class StringObject : public Object
	{
	public:
		StringObject(std::string value);
		~StringObject() = default;

		std::string value;

		ObjectType type() const override;
		std::string inspect() const override;
	};
}