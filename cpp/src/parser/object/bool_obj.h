#pragma once

#include "base_obj.h"

namespace interp::object
{
	class BooleanObject : public Object
	{
	public:
		BooleanObject(bool value);
		~BooleanObject() = default;

		bool value;

		ObjectType type() const override;
		std::string inspect() const override;
	};
}