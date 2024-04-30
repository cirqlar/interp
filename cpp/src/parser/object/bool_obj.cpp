#include "bool_obj.h"

namespace interp::object
{
	BooleanObject::BooleanObject(bool value)
		: value(value)
	{
	}

	ObjectType BooleanObject::type() const
	{
		return ObjectType::BooleanObject;
	}

	std::string BooleanObject::inspect()
	{
		return value ? "true" : "false";
	}
}