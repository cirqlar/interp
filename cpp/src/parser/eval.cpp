#include "eval.h"

namespace interp::eval
{
	auto TRUE = std::shared_ptr<interp::object::BooleanObject>(new interp::object::BooleanObject(true));
	auto FALSE = std::shared_ptr<interp::object::BooleanObject>(new interp::object::BooleanObject(false));
	auto NULL_OBJ = std::shared_ptr<interp::object::Null>(new interp::object::Null());

	std::shared_ptr<interp::object::Object> eval(const interp::ast::Node* node)
	{
		switch (node->type())
		{
		case interp::ast::NodeType::Program:
			if (auto literal = dynamic_cast<const interp::ast::Program*>(node))
			{
				return eval_statments(literal->statements, true);
			}
			return nullptr;
		case interp::ast::NodeType::BlockExpression:
			if (auto literal = dynamic_cast<const interp::ast::BlockExpression*>(node))
			{
				return eval_statments(literal->statements);
			}
			return nullptr;
		case interp::ast::NodeType::BooleanExpression:
			if (auto literal = dynamic_cast<const interp::ast::BooleanLiteral*>(node))
			{
				return literal->value ? TRUE : FALSE;
			}
			return nullptr;
			//case interp::ast::NodeType::CallExpression:
			//	break;
		case interp::ast::NodeType::ExpressionStatment:
			if (auto literal = dynamic_cast<const interp::ast::ExpressionStatement*>(node))
			{
				return eval(literal->expression.get());
			}
			return nullptr;
			//case interp::ast::NodeType::FunctionLiteral:
			//	break;
			//case interp::ast::NodeType::Identifier:
			//	break;
		case interp::ast::NodeType::IfExpression:
			if (auto literal = dynamic_cast<const interp::ast::IfExpression*>(node))
			{
				return eval_if(literal);
			}
			return nullptr;
		case interp::ast::NodeType::InfixExpression:
			if (auto literal = dynamic_cast<const interp::ast::InfixExpression*>(node))
			{
				auto left = eval(literal->left.get());
				if (is_error(left))
					return left;
				auto right = eval(literal->right.get());
				if (is_error(right))
					return right;
				return eval_infix(literal->p_operator, left, right);
			}
			return nullptr;
		case interp::ast::NodeType::IntegerLiteral:
			if (auto literal = dynamic_cast<const interp::ast::IntegerLiteral*>(node))
			{

				return std::shared_ptr<interp::object::Integer>(new interp::object::Integer(literal->value));
			}
			return nullptr;
			//case interp::ast::NodeType::LetStatment:
			//	break;
		case interp::ast::NodeType::PrefixExpression:
			if (auto literal = dynamic_cast<const interp::ast::PrefixExpression*>(node))
			{
				auto right = eval(literal->right.get());
				if (is_error(right))
					return right;
				return eval_prefix(literal->p_operator, right);
			}
			return nullptr;
		case interp::ast::NodeType::ReturnStatment:
			if (auto literal = dynamic_cast<const interp::ast::ReturnStatement*>(node))
			{
				auto inner = eval(literal->return_value.get());
				if (is_error(inner))
					return inner;
				return std::shared_ptr<interp::object::ReturnObject>(
					new interp::object::ReturnObject( inner ));
			}
			return nullptr;
		default:
			return nullptr;
		}
	}

	std::shared_ptr<interp::object::Object> eval_statments(const std::vector<std::shared_ptr<interp::ast::Statement>>& statements, bool unwrap_return)
	{
		std::shared_ptr<interp::object::Object> result = nullptr;

		for (auto& statement : statements)
		{
			result = eval(statement.get());

			if (result->type() == interp::object::ObjectType::ReturnObject)
			{
				if (!unwrap_return)
				{
					return result;
				}

				if (auto r_obj = dynamic_cast<const interp::object::ReturnObject*>(result.get()))
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

	std::shared_ptr<interp::object::Object> eval_prefix(std::string op, const std::shared_ptr<interp::object::Object> right)
	{
		if (op == "!")
			return eval_bang(right);
		else if (op == "-")
			return eval_minus(right);
		else
			return new_error("unknown operator: " + op + interp::object::object_type_to_string(right->type()));
	}

	std::shared_ptr<interp::object::Object> eval_bang(const std::shared_ptr<interp::object::Object> right)
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

	std::shared_ptr<interp::object::Object> eval_minus(const std::shared_ptr<interp::object::Object> right)
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

	std::shared_ptr<interp::object::Object> eval_infix(std::string op, const std::shared_ptr<interp::object::Object> left, const std::shared_ptr<interp::object::Object> right)
	{
		if (left->type() == right->type() && right->type() == interp::object::ObjectType::IntegerObject)
			return eval_int_infix(op, left, right);
		else if (left->type() != right->type())
			return new_error("type mismatch: "
				+ interp::object::object_type_to_string(left->type())
				+ " " + op + " "
				+ interp::object::object_type_to_string(right->type()));
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

	std::shared_ptr<interp::object::Object> eval_int_infix(std::string op, const std::shared_ptr<interp::object::Object> left, const std::shared_ptr<interp::object::Object> right)
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

	std::shared_ptr<interp::object::Object> eval_if(const interp::ast::IfExpression* ifExpr)
	{
		auto condition = eval(ifExpr->condition.get());
		if (is_error(condition))
			return condition;

		if (is_truthy(condition))
		{
			return eval(ifExpr->consequence.get());
		}
		else if (ifExpr->alternative)
		{
			return eval(ifExpr->alternative.get());
		}
		else
		{
			return NULL_OBJ;
		}
	}

	bool is_truthy(const std::shared_ptr<interp::object::Object> obj)
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

	bool is_error(const std::shared_ptr<interp::object::Object> obj)
	{
		return obj->type() == interp::object::ObjectType::ErrorObject;
	}
}