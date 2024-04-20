#include <gtest/gtest.h>
#include <tuple>

#include "parser.h"

void check_parser_errors(interp::parser::Parser p);
void test_let_statment(std::shared_ptr<interp::ast::Statement> stmnt, std::string name);
void test_integer_literal(std::shared_ptr<interp::ast::Expression> expr, int64_t value);
void test_identifier(std::shared_ptr<interp::ast::Expression> expr, std::string value);

TEST(ParserTest, TestLetStatment)
{
	std::string input = R"(
let x = 5;
let y = 10;
let foobar = 838383;
	)";

	auto lex = interp::lexer::Lexer(input);
	auto parse = interp::parser::Parser(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	EXPECT_EQ(3, prog->statements.size())
		<< "program.Statements does not contain 3 statements. got=" << std::to_string(prog->statements.size());

	std::vector<std::string> expected = {"x", "y", "foobar"};

	for (size_t i = 0; i < expected.size(); i++)
	{
		auto stmnt = prog->statements.at(i);
		test_let_statment(stmnt, expected[i]);
	}
}

TEST(ParserTest, TestReturnStatment)
{
	std::string input = R"(
return 5;
return 10;
return 838383;
	)";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	EXPECT_EQ(3, prog->statements.size())
		<< "program.Statements does not contain 3 statements. got=" << std::to_string(prog->statements.size());

	for (auto stmnt : prog->statements)
	{
		if (interp::ast::ReturnStatement *retstmnt = dynamic_cast<interp::ast::ReturnStatement *>(stmnt.get()))
		{
			EXPECT_EQ("return", retstmnt->token_literal()) << "token literal not return got " << retstmnt->token_literal();
		}
		else
		{
			EXPECT_TRUE(false) << "stmnt not ReturnStatement";
		}
	}
}

TEST(ParserTest, TestIdentifierExpression)
{
	std::string input = "foobar;";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement *expstmnt = dynamic_cast<interp::ast::ExpressionStatement *>(prog->statements[0].get()))
	{
		if (interp::ast::Identifier *ident = dynamic_cast<interp::ast::Identifier *>(expstmnt->expression.get()))
		{
			EXPECT_EQ("foobar", ident->value) << "value not foobar got " << ident->value;
			EXPECT_EQ("foobar", ident->token_literal()) << "token literal not foobar got " << ident->token_literal();
		}
		else
		{
			EXPECT_TRUE(false) << "expression not Identifier";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestIntegerLiteral)
{
	std::string input = "5;";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement *expstmnt = dynamic_cast<interp::ast::ExpressionStatement *>(prog->statements[0].get()))
	{
		if (interp::ast::IntegerLiteral *intlit = dynamic_cast<interp::ast::IntegerLiteral *>(expstmnt->expression.get()))
		{
			EXPECT_EQ(5LL, intlit->value) << "value not 5 got " << intlit->value;
			EXPECT_EQ("5", intlit->token_literal()) << "token literal not 5 got " << intlit->token_literal();
		}
		else
		{
			EXPECT_TRUE(false) << "expression not IntegerLiteral";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestBooleanExpression)
{
	std::string input = "true;";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::Boolean* ident = dynamic_cast<interp::ast::Boolean*>(expstmnt->expression.get()))
		{
			EXPECT_EQ(true, ident->value) << "value not true got " << ident->value;
			EXPECT_EQ("true", ident->token_literal()) << "token literal not true got " << ident->token_literal();
		}
		else
		{
			EXPECT_TRUE(false) << "expression not Boolean";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestPrefixExpression)
{
	std::tuple<std::string, std::string, int64_t> expected[] = {
		std::tuple("!5", "!", 5),
		std::tuple("-15", "-", 15),
	};

	for (auto tt : expected)
	{

		interp::lexer::Lexer lex(std::get<0>(tt));
		interp::parser::Parser parse(lex);

		auto prog = parse.parse_program();
		check_parser_errors(parse);

		ASSERT_EQ(1, prog->statements.size())
			<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

		if (interp::ast::ExpressionStatement *expstmnt = dynamic_cast<interp::ast::ExpressionStatement *>(prog->statements[0].get()))
		{
			if (interp::ast::PrefixExpression *pref = dynamic_cast<interp::ast::PrefixExpression *>(expstmnt->expression.get()))
			{

				EXPECT_EQ(std::get<1>(tt), pref->p_operator) << "operatior not " << std::get<1>(tt) << " got " << pref->p_operator;

				test_integer_literal(pref->right, std::get<2>(tt));
			}
			else
			{
				EXPECT_TRUE(false) << "expression not PrefixExpression";
			}
		}
		else
		{
			EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
		}
	}
}

TEST(ParserTest, TestInfixExpression)
{
	std::tuple<std::string, int64_t, std::string, int64_t> expected[] = {
		std::tuple("5 + 5;", 5, "+", 5),
		std::tuple("5 - 5;", 5, "-", 5),
		std::tuple("5 * 5;", 5, "*", 5),
		std::tuple("5 / 5;", 5, "/", 5),
		std::tuple("5 >= 5;", 5, ">=", 5),
		std::tuple("5 <= 5;", 5, "<=", 5),
		std::tuple("5 > 5;", 5, ">", 5),
		std::tuple("5 < 5;", 5, "<", 5),
		std::tuple("5 == 5;", 5, "==", 5),
		std::tuple("5 != 5;", 5, "!=", 5),
	};

	for (auto tt : expected)
	{

		interp::lexer::Lexer lex(std::get<0>(tt));
		interp::parser::Parser parse(lex);

		auto prog = parse.parse_program();
		check_parser_errors(parse);

		ASSERT_EQ(1, prog->statements.size())
			<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

		if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
		{
			if (interp::ast::InfixExpression* pref = dynamic_cast<interp::ast::InfixExpression*>(expstmnt->expression.get()))
			{
				test_integer_literal(pref->left, std::get<1>(tt));

				EXPECT_EQ(std::get<2>(tt), pref->p_operator) << "operatior not " << std::get<2>(tt) << " got " << pref->p_operator;

				test_integer_literal(pref->right, std::get<3>(tt));
			}
			else
			{
				EXPECT_TRUE(false) << "expression not InfixExpression";
			}
		}
		else
		{
			EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
		}
	}
}

TEST(ParserTest, TestOperatorPrecedence)
{
	std::tuple<std::string, std::string> expected[] = {
		std::tuple("true", "true"),
		std::tuple("false", "false"),
		std::tuple("3 > 5 == false", "((3 > 5) == false)"),
		std::tuple("3 < 5 == true", "((3 < 5) == true)"),

		std::tuple("1 + (2 + 3) + 4", "((1 + (2 + 3)) + 4)"),
		/*std::tuple("(5 + 5) * 2", "((5 + 5) * 2)"),
		std::tuple("2 / (5 + 5)", "(2 / (5 + 5))"),
		std::tuple("-(5 + 5)", "(-(5 + 5))"),
		std::tuple("!(true == true)", "(!(true == true))"),*/
	};

	for (auto tt : expected)
	{

		interp::lexer::Lexer lex(std::get<0>(tt));
		interp::parser::Parser parse(lex);

		auto prog = parse.parse_program();
		check_parser_errors(parse);

		ASSERT_EQ(1, prog->statements.size())
			<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

		EXPECT_EQ(std::get<1>(tt), prog->statements[0]->string());
	}
}

void check_parser_errors(interp::parser::Parser p)
{
	auto errors = p.get_errors();
	if (errors.size() == 0)
	{
		return;
	}

	EXPECT_TRUE(false) << "parser has " << std::to_string(errors.size()) << " errors";
	for (auto error : errors)
	{
		EXPECT_TRUE(false) << "parser error: " << error;
	}
	ASSERT_TRUE(false);
}

void test_let_statment(std::shared_ptr<interp::ast::Statement> stmnt, std::string name)
{
	ASSERT_EQ("let", stmnt->token_literal()) << "stmnt literal not 'let'. got=" << stmnt->token_literal();

	if (interp::ast::LetStatement *letstmnt = dynamic_cast<interp::ast::LetStatement *>(stmnt.get()))
	{
		ASSERT_EQ(name, letstmnt->name.value) << "name not " << name << " got " << letstmnt->name.value;
		ASSERT_EQ(name, letstmnt->name.token_literal()) << "name.token_literal not " << name << " got " << letstmnt->name.token_literal();
	}
	else
	{
		ASSERT_TRUE(false) << "stmnt not LetStatement";
	}
}

void test_integer_literal(std::shared_ptr<interp::ast::Expression> expr, int64_t value)
{
	if (interp::ast::IntegerLiteral* intlit = dynamic_cast<interp::ast::IntegerLiteral*>(expr.get()))
	{

		EXPECT_EQ(value, intlit->value) << "value not " << value << " got " << intlit->value;
	}
	else
	{
		EXPECT_TRUE(false) << "expression not IntegerLiteral";
	}
}

void test_identifier(std::shared_ptr<interp::ast::Expression> expr, std::string value)
{
	if (interp::ast::Identifier* ident = dynamic_cast<interp::ast::Identifier*>(expr.get()))
	{
		EXPECT_EQ(value, ident->value) << "value not " << value << " got " << ident->value;
		EXPECT_EQ(value, ident->token_literal()) << "token literal not " << value << " got " << ident->token_literal();
	}
	else
	{
		EXPECT_TRUE(false) << "expression not Identifier";
	}
}
