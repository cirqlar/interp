#include "node.h"

namespace interp::ast
{
	std::string node_type_to_string(NodeType node_type)
	{
		switch (node_type)
		{
		case interp::ast::NodeType::Program:
			return "Program";
		case interp::ast::NodeType::BlockExpression:
			return "BlockExpression";
		case interp::ast::NodeType::BooleanExpression:
			return "BooleanExpression";
		case interp::ast::NodeType::CallExpression:
			return "CallExpression";
		case interp::ast::NodeType::ExpressionStatment:
			return "ExpressionStatment";
		case interp::ast::NodeType::FunctionLiteral:
			return "FunctionLiteral";
		case interp::ast::NodeType::Identifier:
			return "Identifier";
		case interp::ast::NodeType::IfExpression:
			return "IfExpression";
		case interp::ast::NodeType::InfixExpression:
			return "InfixExpression";
		case interp::ast::NodeType::IntegerLiteral:
			return "IntegerLiteral";
		case interp::ast::NodeType::LetStatment:
			return "LetStatment";
		case interp::ast::NodeType::PrefixExpression:
			return "PrefixExpression";
		case interp::ast::NodeType::ReturnStatment:
			return "ReturnStatment";
		default:
			return "Unknown Type";
		}
	}
}
