#include "builtin_fn.h"

namespace interp::object
{
	BuiltinFnObject::BuiltinFnObject(BuiltinFn value)
		: value(value)
	{
	}

	ObjectType BuiltinFnObject::type() const
	{
		return ObjectType::BuiltinFnObject;
	}

	std::string BuiltinFnObject::inspect() const
	{
		return "builtin function";
	}
}