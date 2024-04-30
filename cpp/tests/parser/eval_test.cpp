#include <gtest/gtest.h>

#include "token.h"
#include "parser.h"
#include "eval.h"

std::shared_ptr<interp::object::Object> test_eval(std::string input);
bool test_int_obj(const interp::object::Object* in_object, int64_t expected);
bool test_bool_obj(const interp::object::Object* in_object, bool expected);

TEST(EvalTest, TestEvalIntegerExpression)
{
	std::pair<std::string, int64_t> expected[] = {
		std::pair("5", 5),
		std::pair("10", 10),
		std::pair("-5", -5),
		std::pair("-10", -10),
		std::pair("5 + 5 + 5 + 5 - 10", 10),
		std::pair("2 * 2 * 2 * 2 * 2", 32),
		std::pair("-50 + 100 + -50", 0),
		std::pair("5 * 2 + 10", 20),
		std::pair("5 + 2 * 10", 25),
		std::pair("20 + 2 * -10", 0),
		std::pair("50 / 2 * 2 + 10", 60),
		std::pair("2 * (5 + 10)", 30),
		std::pair("3 * 3 * 3 + 10", 37),
		std::pair("3 * (3 * 3) + 10", 37),
		std::pair("(5 + 10 * 2 + 15 / 3) * 2 + -10", 50),
	};

	for (auto& tt : expected)
	{
		auto obj = test_eval(tt.first);
		test_int_obj(obj.get(), tt.second);
	}
}

TEST(EvalTest, TestEvalBooleanExpression)
{
	std::pair<std::string, bool> expected[] = {
		std::pair("true", true),
		std::pair("false", false),

		std::pair("true == true", true),
		std::pair("false == false", true),
		std::pair("true == false", false),
		std::pair("true != false", true),
		std::pair("false != true", true),
		std::pair("(1 < 2) == true", true),
		std::pair("(1 < 2) == false", false),
		std::pair("(1 > 2) == true", false),
		std::pair("(1 > 2) == false", true),
	};

	for (auto& tt : expected)
	{
		auto obj = test_eval(tt.first);
		test_bool_obj(obj.get(), tt.second);
	}
}

TEST(EvalTest, TestBangOperator)
{
	std::pair<std::string, bool> expected[] = {
		std::pair("!true", false),
		std::pair("!false", true),
		std::pair("!5", false),
		std::pair("!!true", true),
		std::pair("!!false", false),
		std::pair("!!5", true),

		std::pair("1 < 2", true),
		std::pair("1 > 2", false),
		std::pair("1 < 1", false),
		std::pair("1 > 1", false),
		std::pair("1 == 1", true),
		std::pair("1 != 1", false),
		std::pair("1 == 2", false),
		std::pair("1 != 2", true),

		std::pair("1 <= 2", true),
		std::pair("1 >= 2", false),
		std::pair("1 <= 1", true),
		std::pair("1 >= 1", true),
	};

	for (auto& tt : expected)
	{
		auto obj = test_eval(tt.first);
		test_bool_obj(obj.get(), tt.second);
	}
}

std::shared_ptr<interp::object::Object> test_eval(std::string input)
{
	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();

	return interp::eval::eval(prog.get());
}

bool test_int_obj(const interp::object::Object* in_object, int64_t expected)
{
	if (const interp::object::Integer* obj = dynamic_cast<const interp::object::Integer*>(in_object))
	{
		if (obj->value != expected)
		{
			EXPECT_TRUE(false) << "Integer has wrong value. Expected " << expected << " got " << obj->value;
			return false;
		}
		else
			return true;
	}
	else
	{
		EXPECT_TRUE(false) << "Object is not an Interger, type is " << interp::object::object_type_to_string(in_object->type());
		return false;
	}
}

bool test_bool_obj(const interp::object::Object* in_object, bool expected)
{
	if (const interp::object::BooleanObject* obj = dynamic_cast<const interp::object::BooleanObject*>(in_object))
	{
		if (obj->value != expected)
		{
			EXPECT_TRUE(false) << "Integer has wrong value. Expected " << expected << " got " << obj->value;
			return false;
		}
		else
			return true;
	}
	else
	{
		EXPECT_TRUE(false) << "Object is not an Boolean, type is " << interp::object::object_type_to_string(in_object->type());
		return false;
	}
}

