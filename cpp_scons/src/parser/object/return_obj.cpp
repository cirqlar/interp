#include "return_obj.h"

namespace interp::object
{
	ReturnObject::ReturnObject(std::shared_ptr<Object> value)
		: value(value)
	{
	}

	ObjectType ReturnObject::type() const
	{
		return ObjectType::ReturnObject;
	}

	std::string ReturnObject::inspect() const
	{
		return this->value->inspect();
	}
}