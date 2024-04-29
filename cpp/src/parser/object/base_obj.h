#pragma once

#include <iostream>

namespace interp::object
{
	enum struct ObjectType
	{
		IntegerObject,
		BooleanObject,
		NullObject,
	};

	std::string object_type_to_string(ObjectType object_type);

	class Object
	{
	public:
		virtual ~Object() = default;

		virtual ObjectType type() = 0;
		virtual std::string inspect() = 0;
	};
}