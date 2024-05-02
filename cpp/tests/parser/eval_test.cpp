#include <gtest/gtest.h>

#include "token.h"
#include "parser.h"
#include "eval.h"

std::shared_ptr<interp::object::Object> test_eval(std::string input);
bool test_int_obj(const interp::object::Object* in_object, int64_t expected, std::string input);
bool test_bool_obj(const interp::object::Object* in_object, bool expected);
bool test_null_obj(const interp::object::Object* in_object);
bool test_error(const interp::object::Object* in_object, std::string expected);

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

		std::pair("return 10;", 10),
		std::pair("return 10; 9;", 10),
		std::pair("return 2 * 5; 9;", 10),
		std::pair("9; return 2 * 5; 9;", 10),
		std::pair("if (10 > 1) { if (10 > 1) { return 10; } return 1; }", 10),

		std::pair("let a = 5; a;", 5),
		std::pair("let a = 5 * 5; a;", 25),
		std::pair("let a = 5; let b = a; b;", 5),
		std::pair("let a = 5; let b = a; let c = a + b + 5; c;", 15),

		std::pair("let identity = fn(x) { x; }; identity(5);", 5),
		std::pair("let identity = fn(x) { return x; }; identity(5);", 5),
		std::pair("let double = fn(x) { x * 2; }; double(5);", 10),
		std::pair("let add = fn(x, y) { x + y; }; add(5, 5);", 10),
		std::pair("let add = fn(x, y) { x + y; }; add(5 + 5, add(5, 5));", 20),
		std::pair("fn(x) { x; }(5)", 5),
		std::pair(R"(
let newAdder = fn(x) {
	fn(y) { x + y };
};
let addTwo = newAdder(2);
addTwo(2);
		)", 4),
	};

	for (auto& tt : expected)
	{
		auto obj = test_eval(tt.first);
		test_int_obj(obj.get(), tt.second, tt.first);
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

TEST(EvalTest, TestIfElseExpressions)
{
	auto NULL_OBJ = std::shared_ptr<interp::object::Null>(new interp::object::Null());
	auto FIVE_OBJ = std::shared_ptr<interp::object::Integer>(new interp::object::Integer(5));
	auto FIFTEEN_OBJ = std::shared_ptr<interp::object::Integer>(new interp::object::Integer(15));

	std::pair<std::string, std::shared_ptr<interp::object::Object>> expected[] = {
		std::pair("if (true) { 5 }", FIVE_OBJ),
		std::pair("if (false) { 5 }", NULL_OBJ),
		std::pair("if (1) { 5 }", FIVE_OBJ),
		std::pair("if (1 < 2) { 5 }", FIVE_OBJ),
		std::pair("if (1 > 2) { 5 }", NULL_OBJ),
		std::pair("if (1 > 2) { 5 } else { 15 }", FIFTEEN_OBJ),
		std::pair("if (1 < 2) { 5 } else { 15 }", FIVE_OBJ),
	};

	for (auto& tt : expected)
	{
		auto obj = test_eval(tt.first);

		switch (tt.second->type())
		{
		case interp::object::ObjectType::IntegerObject:
			test_int_obj(obj.get(), tt.second == FIVE_OBJ ? 5 : 15, tt.first);
			continue;
		/*case interp::object::ObjectType::BooleanObject:
			break;*/
		case interp::object::ObjectType::NullObject:
			test_null_obj(obj.get());
			continue;
		default:
			continue;
		}
	}
}

TEST(EvalTest, TestErrorHandling)
{
	std::pair<std::string, std::string> expected[] = {
		std::pair("5 + true;", "type mismatch: INTEGER + BOOLEAN"),
		std::pair("5 + true; 5;", "type mismatch: INTEGER + BOOLEAN"),
		std::pair("-true", "unknown operator: -BOOLEAN"),
		std::pair("true + false;", "unknown operator: BOOLEAN + BOOLEAN"),
		std::pair("5; true + false; 5", "unknown operator: BOOLEAN + BOOLEAN"),
		std::pair("if (10 > 1) { true + false; }", "unknown operator: BOOLEAN + BOOLEAN"),
		std::pair("if (10 > 1) { if (10 > 1) { return true + false; } return 1; }", "unknown operator: BOOLEAN + BOOLEAN"),
		std::pair("foobar", "identifier not found: foobar"),
	};

	for (auto& tt : expected)
	{
		auto obj = test_eval(tt.first);
		test_error(obj.get(), tt.second);
	}
}

std::shared_ptr<interp::object::Object> test_eval(std::string input)
{
	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();

	auto env = interp::object::Environment::new_env(nullptr);
	return interp::eval::eval(prog, env);
}

bool test_int_obj(const interp::object::Object* in_object, int64_t expected, std::string input)
{
	if (auto obj = dynamic_cast<const interp::object::Integer*>(in_object))
	{
		if (obj->value != expected)
		{
			EXPECT_TRUE(false) << "Integer has wrong value. Expected " << expected << " got " << obj->value << "\nFailed for: " << input;
			return false;
		}
		else
			return true;
	}
	else
	{
		EXPECT_TRUE(false) 
			<< "Object is not an Interger, type is " 
			<< interp::object::object_type_to_string(in_object->type())
			<< "\nFailed for " << input;
		return false;
	}
}

bool test_bool_obj(const interp::object::Object* in_object, bool expected)
{
	if (auto obj = dynamic_cast<const interp::object::BooleanObject*>(in_object))
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


bool test_null_obj(const interp::object::Object* in_object)
{
	if (auto obj = dynamic_cast<const interp::object::Null*>(in_object))
	{
		return true;
	}
	else
	{
		EXPECT_TRUE(false) << "Object is not an Null, type is " << interp::object::object_type_to_string(in_object->type());
		return false;
	}
}

bool test_error(const interp::object::Object* in_object, std::string expected)
{
	if (auto obj = dynamic_cast<const interp::object::ErrorObject*>(in_object))
	{
		if (obj->message != expected)
		{
			EXPECT_TRUE(false)
				<< "Got wrong error message. Expected: "
				<< expected
				<< " \nGot "
				<< obj->message;
			return false;
		}

		return true;
	}
	else
	{
		EXPECT_TRUE(false)
			<< "Object is not an Error, got "
			<< interp::object::object_type_to_string(in_object->type())
			<< ": "
			<< in_object->inspect();

		return false;
	}
}