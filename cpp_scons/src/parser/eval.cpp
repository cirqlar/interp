#include "eval.h"
#include "builtins/builtins.h"

namespace interp::eval
{
	auto TRUE = std::shared_ptr<interp::object::BooleanObject>(new interp::object::BooleanObject(true));
	auto FALSE = std::shared_ptr<interp::object::BooleanObject>(new interp::object::BooleanObject(false));
	auto NULL_OBJ = std::shared_ptr<interp::object::Null>(new interp::object::Null());

	std::shared_ptr<interp::object::Object> eval(std::shared_ptr<interp::ast::Node> node, std::shared_ptr<interp::object::Environment>& env)
	{
		switch (node->type())
		{
		case interp::ast::NodeType::Program:
			if (auto literal = dynamic_cast<interp::ast::Program*>(node.get()))
			{
				return eval_statments(literal->statements, env, true);
			}
			return nullptr;
		case interp::ast::NodeType::BlockExpression:
			if (auto literal = dynamic_cast<interp::ast::BlockExpression*>(node.get()))
			{
				auto new_env = interp::object::Environment::new_env(env);
				return eval_statments(literal->statements, new_env);
			}
			return nullptr;
		case interp::ast::NodeType::BooleanExpression:
			if (auto literal = dynamic_cast<interp::ast::BooleanLiteral*>(node.get()))
			{
				return literal->value ? TRUE : FALSE;
			}
			return nullptr;
		case interp::ast::NodeType::CallExpression:
			if (auto literal = dynamic_cast<interp::ast::CallExpression*>(node.get()))
			{
				auto fn = eval(literal->function, env);
				if (is_error(fn))
					return fn;

				auto args = eval_expressions(literal->args, env);
				if (args.size() == 1 && is_error(args[0]))
					return args[0];

				return apply_fn(fn, args);
			}
			return nullptr;
		case interp::ast::NodeType::ExpressionStatment:
			if (auto literal = dynamic_cast<interp::ast::ExpressionStatement*>(node.get()))
			{
				return eval(literal->expression, env);
			}
			return nullptr;
		case interp::ast::NodeType::FunctionLiteral:
			if (auto literal = dynamic_cast<interp::ast::FunctionLiteral*>(node.get()))
			{
				return std::shared_ptr<interp::object::FunctionObject>(
					new interp::object::FunctionObject(literal, env) );
			}
			return nullptr;
		case interp::ast::NodeType::Identifier:
			if (auto literal = dynamic_cast<interp::ast::Identifier*>(node.get()))
			{
				auto obj = env->get(literal->value);

				if (obj)
					return obj;
				else
					return new_error("identifier not found: " + literal->value);
			}
			return nullptr;
		case interp::ast::NodeType::IfExpression:
			if (auto literal = dynamic_cast<interp::ast::IfExpression*>(node.get()))
			{
				return eval_if(literal, env);
			}
			return nullptr;
		case interp::ast::NodeType::InfixExpression:
			if (auto literal = dynamic_cast<interp::ast::InfixExpression*>(node.get()))
			{
				auto left = eval(literal->left, env);
				if (is_error(left))
					return left;
				auto right = eval(literal->right, env);
				if (is_error(right))
					return right;
				return eval_infix(literal->p_operator, left, right);
			}
			return nullptr;
		case interp::ast::NodeType::IntegerLiteral:
			if (auto literal = dynamic_cast<interp::ast::IntegerLiteral*>(node.get()))
			{

				return std::shared_ptr<interp::object::Integer>(new interp::object::Integer(literal->value));
			}
			return nullptr;
		case interp::ast::NodeType::LetStatment:
			if (auto literal = dynamic_cast<interp::ast::LetStatement*>(node.get()))
			{
				auto inner = eval(literal->value, env);
				if (is_error(inner))
					return inner;

				return env->set(literal->name.value, inner);
			}
			return nullptr;
		case interp::ast::NodeType::PrefixExpression:
			if (auto literal = dynamic_cast<interp::ast::PrefixExpression*>(node.get()))
			{
				auto right = eval(literal->right, env);
				if (is_error(right))
					return right;
				return eval_prefix(literal->p_operator, right);
			}
			return nullptr;
		case interp::ast::NodeType::ReturnStatment:
			if (auto literal = dynamic_cast<interp::ast::ReturnStatement*>(node.get()))
			{
				auto inner = eval(literal->return_value, env);
				if (is_error(inner))
					return inner;
				return std::shared_ptr<interp::object::ReturnObject>(
					new interp::object::ReturnObject( inner ));
			}
			return nullptr;
		case interp::ast::NodeType::StringLiteral:
			if (auto literal = dynamic_cast<interp::ast::StringLiteral*>(node.get()))
			{
				return std::shared_ptr<interp::object::StringObject>(
					new interp::object::StringObject( literal->value ));
			}
			return nullptr;
		default:
			return nullptr;
		}
	}

	std::shared_ptr<interp::object::Object> eval_statments(std::vector<std::shared_ptr<interp::ast::Statement>>& statements, std::shared_ptr<interp::object::Environment>& env, bool unwrap_return)
	{
		std::shared_ptr<interp::object::Object> result = nullptr;

		for (auto& statement : statements)
		{
			result = eval(statement, env);

			if (result->type() == interp::object::ObjectType::ReturnObject)
			{
				if (!unwrap_return)
				{
					return result;
				}

				if (auto r_obj = dynamic_cast<interp::object::ReturnObject*>(result.get()))
				{
					return r_obj->value;
				}
			}
			else if (result->type() == interp::object::ObjectType::ErrorObject)
			{
				return result;
			}
		}

		return result;
	}

	std::vector<std::shared_ptr<interp::object::Object>> eval_expressions(std::vector<std::shared_ptr<interp::ast::Expression>>& expressions, std::shared_ptr<interp::object::Environment>& env)
	{
		std::vector<std::shared_ptr<interp::object::Object>> results({});

		for (auto& expr : expressions)
		{
			auto evaled = eval(expr, env);

			if (is_error(evaled))
			{
				results.clear();
				results.emplace_back(evaled);
				return results;
			}

			results.emplace_back(evaled);
		}

		return results;
	}

	std::shared_ptr<interp::object::Object> eval_prefix(std::string& op, std::shared_ptr<interp::object::Object>& right)
	{
		if (op == "!")
			return eval_bang(right);
		else if (op == "-")
			return eval_minus(right);
		else
			return new_error("unknown operator: " + op + interp::object::object_type_to_string(right->type()));
	}

	std::shared_ptr<interp::object::Object> eval_bang(std::shared_ptr<interp::object::Object>& right)
	{
		switch (right->type())
		{
		case interp::object::ObjectType::BooleanObject:
			if (auto bool_obj = dynamic_cast<interp::object::BooleanObject*>(right.get()))
			{
				return bool_obj->value ? FALSE : TRUE;
			}
			[[fallthrough]];
		case interp::object::ObjectType::NullObject:
			return TRUE;
		default:
			return FALSE;
		}
	}

	std::shared_ptr<interp::object::Object> eval_minus(std::shared_ptr<interp::object::Object>& right)
	{
		switch (right->type())
		{
		case interp::object::ObjectType::IntegerObject:
			if (auto int_obj = dynamic_cast<interp::object::Integer*>(right.get()))
			{
				return std::shared_ptr<interp::object::Integer>(new interp::object::Integer(-int_obj->value));
			}
			[[fallthrough]];
		default:
			return new_error("unknown operator: -" + interp::object::object_type_to_string(right->type()));
		}
	}

	std::shared_ptr<interp::object::Object> eval_infix(std::string& op, std::shared_ptr<interp::object::Object>& left, std::shared_ptr<interp::object::Object>& right)
	{
		if (left->type() != right->type())
			return new_error("type mismatch: "
				+ interp::object::object_type_to_string(left->type())
				+ " " + op + " "
				+ interp::object::object_type_to_string(right->type()));
		else if (right->type() == interp::object::ObjectType::IntegerObject)
			return eval_int_infix(op, left, right);
		if (right->type() == interp::object::ObjectType::StringObject)
			return eval_string_infix(op, left, right);
		else if (op == "==")
			return left == right ? TRUE : FALSE;
		else if (op == "!=")
			return left != right ? TRUE : FALSE;
		else
			return new_error("unknown operator: " 
				+ interp::object::object_type_to_string(left->type())
				+ " " + op + " "
				+ interp::object::object_type_to_string(right->type()));
	}

	std::shared_ptr<interp::object::Object> eval_int_infix(std::string& op, std::shared_ptr<interp::object::Object>& left, std::shared_ptr<interp::object::Object>& right)
	{
		if (auto right_obj = dynamic_cast<interp::object::Integer*>(right.get()))
		{
			if (auto left_obj = dynamic_cast<interp::object::Integer*>(left.get()))
			{
				int64_t result = 0;
				if (op == "-")
					result = left_obj->value - right_obj->value;
				else if (op == "+")
					result = left_obj->value + right_obj->value;
				else if (op == "*")
					result = left_obj->value * right_obj->value;
				else if (op == "/")
					result = left_obj->value / right_obj->value;
				else if (op == "<")
					return left_obj->value < right_obj->value ? TRUE : FALSE;
				else if (op == ">")
					return left_obj->value > right_obj->value ? TRUE : FALSE;
				else if (op == "<=")
					return left_obj->value <= right_obj->value ? TRUE : FALSE;
				else if (op == ">=")
					return left_obj->value >= right_obj->value ? TRUE : FALSE;
				else if (op == "==")
					return left_obj->value == right_obj->value ? TRUE : FALSE;
				else if (op == "!=")
					return left_obj->value != right_obj->value ? TRUE : FALSE;
				else
					return new_error("unknown operator: "
						+ interp::object::object_type_to_string(left_obj->type())
						+ " " + op + " "
						+ interp::object::object_type_to_string(right_obj->type()));
				
				return std::shared_ptr<interp::object::Integer>(new interp::object::Integer(result));
			}
		}
		
		return new_error("unknown operator: "
			+ interp::object::object_type_to_string(left->type())
			+ " " + op + " "
			+ interp::object::object_type_to_string(right->type()));
	}
	
	std::shared_ptr<interp::object::Object> eval_string_infix(std::string& op, std::shared_ptr<interp::object::Object>& left, std::shared_ptr<interp::object::Object>& right)
	{
		if (auto right_obj = dynamic_cast<interp::object::StringObject*>(right.get()))
		{
			if (auto left_obj = dynamic_cast<interp::object::StringObject*>(left.get()))
			{
				if (op == "+")
					return std::shared_ptr<interp::object::StringObject>(new interp::object::StringObject(left_obj->value + right_obj->value));
				else
					return new_error("unknown operator: "
						+ interp::object::object_type_to_string(left_obj->type())
						+ " " + op + " "
						+ interp::object::object_type_to_string(right_obj->type()));
			}
		}
		
		return new_error("unknown operator: "
			+ interp::object::object_type_to_string(left->type())
			+ " " + op + " "
			+ interp::object::object_type_to_string(right->type()));
	}

	std::shared_ptr<interp::object::Object> eval_if(interp::ast::IfExpression* ifExpr, std::shared_ptr<interp::object::Environment>& env)
	{
		auto condition = eval(ifExpr->condition, env);
		if (is_error(condition))
			return condition;

		if (is_truthy(condition))
		{
			return eval(ifExpr->consequence, env);
		}
		else if (ifExpr->alternative)
		{
			return eval(ifExpr->alternative, env);
		}
		else
		{
			return NULL_OBJ;
		}
	}

	std::shared_ptr<interp::object::Object> apply_fn(std::shared_ptr<interp::object::Object> fn, std::vector<std::shared_ptr<interp::object::Object>>& args)
	{
		if (auto fn_obj = dynamic_cast<interp::object::FunctionObject*>(fn.get()))
		{
			auto env = extend_fn_env(fn_obj, args);
			return eval(fn_obj->body, env);
		}
		else
		{
			return new_error("not a function: " + interp::object::object_type_to_string(fn->type()));
		}
	}

	std::shared_ptr<interp::object::Environment> extend_fn_env(interp::object::FunctionObject* fn, std::vector<std::shared_ptr<interp::object::Object>>& args)
	{
		auto env = interp::object::Environment::new_env(fn->environment);

		for (size_t i = 0; i < fn->params.size(); i++)
		{
			env->set(fn->params[i]->value, args[i]);
		}

		return env;
	}

	bool is_truthy(std::shared_ptr<interp::object::Object>& obj)
	{
		switch (obj->type())
		{
		case interp::object::ObjectType::BooleanObject:
			return obj == TRUE;
		case interp::object::ObjectType::NullObject:
			return false;
		default:
			return true;
		}
	}

	std::shared_ptr<interp::object::ErrorObject> new_error(std::string message)
	{
		return std::shared_ptr<interp::object::ErrorObject>(new interp::object::ErrorObject(message));
	}

	bool is_error(std::shared_ptr<interp::object::Object>& obj)
	{
		return obj->type() == interp::object::ObjectType::ErrorObject;
	}
}