#include "error_obj.h"

namespace interp::object
{
	ErrorObject::ErrorObject(std::string message)
		: message(message)
	{
	}

	ObjectType ErrorObject::type() const
	{
		return ObjectType::ErrorObject;
	}

	std::string ErrorObject::inspect() const
	{
		return "ERROR: " + this->message;
	}
}