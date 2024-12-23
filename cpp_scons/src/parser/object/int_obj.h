#pragma once

#include "base_obj.h"

namespace interp::object
{
	class Integer : public Object
	{
	public:
		Integer(int64_t value);
		~Integer() = default;

		int64_t value;

		ObjectType type() const override;
		std::string inspect() const override;
	};
}