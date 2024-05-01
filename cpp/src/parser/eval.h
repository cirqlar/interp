#pragma once

#include "ast.h"
#include "object.h"

namespace interp::eval
{
	std::shared_ptr<interp::object::Object> eval(const interp::ast::Node* node);

	std::shared_ptr<interp::object::Object> eval_statments(const std::vector<std::shared_ptr<interp::ast::Statement>>& statments, bool unwrap_return = false);
	std::shared_ptr<interp::object::Object> eval_prefix(std::string op, const std::shared_ptr<interp::object::Object> right);
	std::shared_ptr<interp::object::Object> eval_bang(const std::shared_ptr<interp::object::Object> right);
	std::shared_ptr<interp::object::Object> eval_minus(const std::shared_ptr<interp::object::Object> right);
	std::shared_ptr<interp::object::Object> eval_infix(std::string op, const std::shared_ptr<interp::object::Object> left, const std::shared_ptr<interp::object::Object> right);
	std::shared_ptr<interp::object::Object> eval_int_infix(std::string op, const std::shared_ptr<interp::object::Object> left, const std::shared_ptr<interp::object::Object> right);
	std::shared_ptr<interp::object::Object> eval_if(const interp::ast::IfExpression* ifExpr);
	bool is_truthy(const std::shared_ptr<interp::object::Object> obj);
	std::shared_ptr<interp::object::ErrorObject> new_error(std::string message);
	bool is_error(const std::shared_ptr<interp::object::Object> obj);
}