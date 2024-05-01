#pragma once

#include "base_obj.h"

namespace interp::object
{
	class ReturnObject : public Object
	{
	public:
		ReturnObject(std::shared_ptr<Object> value);
		~ReturnObject() = default;

		std::shared_ptr<Object> value;

		ObjectType type() const override;
		std::string inspect() override;
	};
}