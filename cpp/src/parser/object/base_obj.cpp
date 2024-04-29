#include "base_obj.h"

namespace interp::object
{
	std::string object_type_to_string(ObjectType object_type)
	{
		switch (object_type)
		{
		case interp::object::ObjectType::IntegerObject:
			return "IntegerObject";
		case interp::object::ObjectType::BooleanObject:
			return "BooleanObject";
		case interp::object::ObjectType::NullObject:
			return "NullObject";
		default:
			return "Unknown Type";
		}
	}
}