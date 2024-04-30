#include "null_obj.h"

namespace interp::object
{
	ObjectType Null::type() const
	{
		return ObjectType::NullObject;
	}

	std::string Null::inspect()
	{
		return "null";
	}
}