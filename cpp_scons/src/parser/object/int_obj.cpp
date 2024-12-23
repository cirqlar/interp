#include "int_obj.h"
#include <string>

namespace interp::object
{
	Integer::Integer(int64_t value)
		: value(value)
	{
	}

	ObjectType Integer::type() const
	{
		return ObjectType::IntegerObject;
	}

	std::string Integer::inspect() const
	{
		return std::to_string(this->value);
	}
}