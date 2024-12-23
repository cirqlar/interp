#pragma once

#include <vector>

#include "base_obj.h"

namespace interp::object
{
	typedef std::shared_ptr<Object>(*BuiltinFn)(std::vector<std::shared_ptr<Object>>);

	class BuiltinFnObject : public Object
	{
	public:
		BuiltinFnObject(BuiltinFn value);
		~BuiltinFnObject() = default;

		BuiltinFn value;

		ObjectType type() const override;
		std::string inspect() const override;
	};
}