#pragma once

#include <iostream>

namespace interp::object
{
	enum struct ObjectType
	{
		IntegerObject,
		BooleanObject,
		NullObject,
		ReturnObject,
		ErrorObject,
		FunctionObject,
		StringObject,
		BuiltinFnObject,
	};

	std::string object_type_to_string(ObjectType object_type);

	class Object
	{
	public:
		virtual ~Object() = default;

		virtual ObjectType type() const = 0;
		virtual std::string inspect() const = 0;
	};
}