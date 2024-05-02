#pragma once

#include <iostream>
#include <map>
#include <memory>

#include "base_obj.h"

namespace interp::object
{
	class Environment
	{
	public:
		Environment(std::shared_ptr<Environment> outer);
		~Environment() = default;

		std::shared_ptr<Object> get(const std::string&);
		std::shared_ptr<Object> set(const std::string&, std::shared_ptr<Object>);

		static std::shared_ptr<Environment> new_env(std::shared_ptr<Environment> outer);

	private:
		std::shared_ptr<Environment> outer;
		std::map<std::string, std::shared_ptr<Object>> store;
	};
}