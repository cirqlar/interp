#include "environment.h"

namespace interp::object
{
	Environment::Environment(std::shared_ptr<Environment> outer)
		: outer(outer)
	{
	}

	std::shared_ptr<Object> Environment::get(const std::string& ident)
	{
		if (this->store.find(ident) == this->store.end())
		{
			if (this->outer)
				return this->outer->get(ident);

			return nullptr;
		}
		else
		{
			return this->store[ident];
		}
	}

	std::shared_ptr<Object> Environment::set(const std::string& ident, std::shared_ptr<Object> obj)
	{
		this->store[ident] = obj;
		return obj;
	}

	std::shared_ptr<Environment> Environment::new_env(std::shared_ptr<Environment> outer)
	{
		return std::shared_ptr<Environment>(new Environment(outer));
	}
}