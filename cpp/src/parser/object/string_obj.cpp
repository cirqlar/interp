#include "string_obj.h"

namespace interp::object
{
	StringObject::StringObject(std::string value)
		: value(value)
	{
	}

	ObjectType StringObject::type() const
	{
		return ObjectType::StringObject;
	}

	std::string StringObject::inspect() const
	{
		return this->value;
	}
}