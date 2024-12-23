#pragma once

#include "ast.h"
#include "object.h"

namespace interp::eval
{
	std::shared_ptr<interp::object::Object> eval(std::shared_ptr<interp::ast::Node> node, std::shared_ptr<interp::object::Environment>& env);

	std::shared_ptr<interp::object::Object> eval_statments(std::vector<std::shared_ptr<interp::ast::Statement>>& statements, std::shared_ptr<interp::object::Environment>& env, bool unwrap_return = false);
	std::vector<std::shared_ptr<interp::object::Object>> eval_expressions(std::vector<std::shared_ptr<interp::ast::Expression>>& expressions, std::shared_ptr<interp::object::Environment>& env);
	std::shared_ptr<interp::object::Object> eval_prefix(std::string& op, std::shared_ptr<interp::object::Object>& right);
	std::shared_ptr<interp::object::Object> eval_bang(std::shared_ptr<interp::object::Object>& right);
	std::shared_ptr<interp::object::Object> eval_minus(std::shared_ptr<interp::object::Object>& right);
	std::shared_ptr<interp::object::Object> eval_infix(std::string& op, std::shared_ptr<interp::object::Object>& left, std::shared_ptr<interp::object::Object>& right);
	std::shared_ptr<interp::object::Object> eval_int_infix(std::string& op, std::shared_ptr<interp::object::Object>& left, std::shared_ptr<interp::object::Object>& right);
	std::shared_ptr<interp::object::Object> eval_string_infix(std::string& op, std::shared_ptr<interp::object::Object>& left, std::shared_ptr<interp::object::Object>& right);
	std::shared_ptr<interp::object::Object> eval_if(interp::ast::IfExpression* ifExpr, std::shared_ptr<interp::object::Environment>& env);
	std::shared_ptr<interp::object::Object> apply_fn(std::shared_ptr<interp::object::Object> fn, std::vector<std::shared_ptr<interp::object::Object>>& args);
	std::shared_ptr<interp::object::Environment> extend_fn_env(interp::object::FunctionObject* fn, std::vector<std::shared_ptr<interp::object::Object>>& args);
	bool is_truthy(std::shared_ptr<interp::object::Object>& obj);
	std::shared_ptr<interp::object::ErrorObject> new_error(std::string message);
	bool is_error(std::shared_ptr<interp::object::Object>& obj);
}