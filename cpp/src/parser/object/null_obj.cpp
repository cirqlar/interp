#include "null_obj.h"

namespace interp::object
{
	ObjectType Null::type()
	{
		return ObjectType::NullObject;
	}

	std::string Null::inspect()
	{
		return "null";
	}
}