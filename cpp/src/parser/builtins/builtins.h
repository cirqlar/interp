#pragma once

#include <iostream>
#include <map>
#include <string>

#include "object/builtin_fn.h"
#include "object/error_obj.h"

namespace interp::eval
{
	std::map<std::string, interp::object::BuiltinFn> builtins({

		// LEN
		std::pair("len", [](std::vector<std::shared_ptr<interp::object::Object>> args) -> std::shared_ptr<interp::object::Object> {
			if (args.size() != 1)
			{
				return std::shared_ptr<interp::object::ErrorObject>(
					new interp::object::ErrorObject("wrong number of arguments. got=" + std::to_string(args.size()) + " want=1"));
			}

			switch (args[0]->type())
			{
			case interp::object::ObjectType::StringObject:
				if (auto str = dynamic_cast<interp::object::StringObject*>(args[0].get()))
				{
					return std::shared_ptr<interp::object::Integer>(
						new interp::object::Integer(str->value.length()));
				}
			default:
				return std::shared_ptr<interp::object::ErrorObject>(
					new interp::object::ErrorObject("argument to `len` not supported, got=" + interp::object::object_type_to_string( args[0]->type() )));
			}
		}),
	});
}