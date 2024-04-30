#pragma once

#include "base_obj.h"

namespace interp::object
{
	class Null : public Object
	{
	public:
		Null() = default;
		~Null() = default;

		ObjectType type() const override;
		std::string inspect() override;
	};
}