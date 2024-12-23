#include "base_obj.h"

namespace interp::object
{
	std::string object_type_to_string(ObjectType object_type)
	{
		switch (object_type)
		{
		case interp::object::ObjectType::IntegerObject:
			return "INTEGER";
		case interp::object::ObjectType::BooleanObject:
			return "BOOLEAN";
		case interp::object::ObjectType::NullObject:
			return "NULL";
		case interp::object::ObjectType::ReturnObject:
			return "ReturnObject";
		case interp::object::ObjectType::ErrorObject:
			return "ErrorObject";
		case interp::object::ObjectType::FunctionObject:
			return "FunctionObject";
		case interp::object::ObjectType::StringObject:
			return "STRING";
		case interp::object::ObjectType::BuiltinFnObject:
			return "BuiltinFnObject";
		default:
			return "Unknown Type";
		}
	}
}