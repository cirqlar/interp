#include <gtest/gtest.h>
#include <tuple>

#include "parser.h"

void check_parser_errors(interp::parser::Parser p);
void test_let_statment(std::shared_ptr<interp::ast::Statement> stmnt, std::string name);
void test_integer_literal(std::shared_ptr<interp::ast::Expression> expr, int64_t value);
void test_identifier(std::shared_ptr<interp::ast::Expression> expr, std::string value);
void test_boolean(std::shared_ptr<interp::ast::Expression> expr, bool value);

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
		if (interp::ast::BooleanLiteral* ident = dynamic_cast<interp::ast::BooleanLiteral*>(expstmnt->expression.get()))
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
		std::tuple("(5 + 5) * 2", "((5 + 5) * 2)"),
		std::tuple("2 / (5 + 5)", "(2 / (5 + 5))"),
		std::tuple("-(5 + 5)", "(-(5 + 5))"),
		std::tuple("!(true == true)", "(!(true == true))"),
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

TEST(ParserTest, TestIfExpressionWithBlock)
{
	std::string input = "if (7 < 5) { x }";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::IfExpression* ifExpr = dynamic_cast<interp::ast::IfExpression*>(expstmnt->expression.get()))
		{
			if (interp::ast::InfixExpression* condition = dynamic_cast<interp::ast::InfixExpression*>(ifExpr->condition.get()))
			{
				test_integer_literal(condition->left, 7);

				EXPECT_EQ("<", condition->p_operator) << "operatior not < got " << condition->p_operator;

				test_integer_literal(condition->right, 5);
			}
			else
			{
				EXPECT_TRUE(false) << "expression not InfixExpression";
			}

			if (interp::ast::BlockExpression* consequence = dynamic_cast<interp::ast::BlockExpression*>(ifExpr->consequence.get()))
			{
				ASSERT_EQ(1, consequence->statements.size())
					<< "consequence.statements does not contain 1 statement. got=" << std::to_string(consequence->statements.size());

				if (interp::ast::ExpressionStatement* stmnt = dynamic_cast<interp::ast::ExpressionStatement*>(consequence->statements[0].get()))
				{
					test_identifier(stmnt->expression, "x");
				}
				else
				{
					EXPECT_TRUE(false) << "statement not ExpressionStatement";
				}
			}
			else
			{
				EXPECT_TRUE(false) << "expression not BlockExpression";
			}
		}
		else
		{
			EXPECT_TRUE(false) << "expression not IfExpression";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestIfExpression)
{
	std::string input = "if (7 < 5) x";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::IfExpression* ifExpr = dynamic_cast<interp::ast::IfExpression*>(expstmnt->expression.get()))
		{
			if (interp::ast::InfixExpression* condition = dynamic_cast<interp::ast::InfixExpression*>(ifExpr->condition.get()))
			{
				test_integer_literal(condition->left, 7);

				EXPECT_EQ("<", condition->p_operator) << "operatior not < got " << condition->p_operator;

				test_integer_literal(condition->right, 5);
			}
			else
			{
				EXPECT_TRUE(false) << "expression not InfixExpression";
			}

			
			test_identifier(ifExpr->consequence, "x");
		}
		else
		{
			EXPECT_TRUE(false) << "expression not IfExpression";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestIfElseExpressionWithBlock)
{
	std::string input = "if (7 < 5) { x } else { x }";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::IfExpression* ifExpr = dynamic_cast<interp::ast::IfExpression*>(expstmnt->expression.get()))
		{
			if (interp::ast::InfixExpression* condition = dynamic_cast<interp::ast::InfixExpression*>(ifExpr->condition.get()))
			{
				test_integer_literal(condition->left, 7);

				EXPECT_EQ("<", condition->p_operator) << "operatior not < got " << condition->p_operator;

				test_integer_literal(condition->right, 5);
			}
			else
			{
				EXPECT_TRUE(false) << "expression not InfixExpression";
			}

			if (interp::ast::BlockExpression* consequence = dynamic_cast<interp::ast::BlockExpression*>(ifExpr->consequence.get()))
			{
				ASSERT_EQ(1, consequence->statements.size())
					<< "consequence.statements does not contain 1 statement. got=" << std::to_string(consequence->statements.size());

				if (interp::ast::ExpressionStatement* stmnt = dynamic_cast<interp::ast::ExpressionStatement*>(consequence->statements[0].get()))
				{
					test_identifier(stmnt->expression, "x");
				}
				else
				{
					EXPECT_TRUE(false) << "statement not ExpressionStatement";
				}
			}
			else
			{
				EXPECT_TRUE(false) << "expression not BlockExpression";
			}


			if (interp::ast::BlockExpression* alternative = dynamic_cast<interp::ast::BlockExpression*>(ifExpr->alternative.get()))
			{
				ASSERT_EQ(1, alternative->statements.size())
					<< "consequence.statements does not contain 1 statement. got=" << std::to_string(alternative->statements.size());

				if (interp::ast::ExpressionStatement* stmnt = dynamic_cast<interp::ast::ExpressionStatement*>(alternative->statements[0].get()))
				{
					test_identifier(stmnt->expression, "x");
				}
				else
				{
					EXPECT_TRUE(false) << "statement not ExpressionStatement";
				}
			}
			else
			{
				EXPECT_TRUE(false) << "expression not BlockExpression";
			}
		}
		else
		{
			EXPECT_TRUE(false) << "expression not IfExpression";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestIfElseExpression)
{
	std::string input = "if (7 < 5) x else y";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::IfExpression* ifExpr = dynamic_cast<interp::ast::IfExpression*>(expstmnt->expression.get()))
		{
			if (interp::ast::InfixExpression* condition = dynamic_cast<interp::ast::InfixExpression*>(ifExpr->condition.get()))
			{
				test_integer_literal(condition->left, 7);

				EXPECT_EQ("<", condition->p_operator) << "operatior not < got " << condition->p_operator;

				test_integer_literal(condition->right, 5);
			}
			else
			{
				EXPECT_TRUE(false) << "expression not InfixExpression";
			}


			test_identifier(ifExpr->consequence, "x");
			test_identifier(ifExpr->alternative, "y");
		}
		else
		{
			EXPECT_TRUE(false) << "expression not IfExpression";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestIfElseIfExpression)
{
	std::string input = "if (7 < 5) x else if (true) y";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::IfExpression* ifExpr = dynamic_cast<interp::ast::IfExpression*>(expstmnt->expression.get()))
		{
			if (interp::ast::InfixExpression* condition = dynamic_cast<interp::ast::InfixExpression*>(ifExpr->condition.get()))
			{
				test_integer_literal(condition->left, 7);

				EXPECT_EQ("<", condition->p_operator) << "operatior not < got " << condition->p_operator;

				test_integer_literal(condition->right, 5);
			}
			else
			{
				EXPECT_TRUE(false) << "expression not InfixExpression";
			}


			test_identifier(ifExpr->consequence, "x");
			if (interp::ast::IfExpression* ElseifExpr = dynamic_cast<interp::ast::IfExpression*>(ifExpr->alternative.get()))
			{
				test_boolean(ElseifExpr->condition, true);
				test_identifier(ElseifExpr->consequence, "y");
			}
		}
		else
		{
			EXPECT_TRUE(false) << "expression not IfExpression";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestFunctionLiteralWithBlock)
{
	std::string input = "fn(x, y) { x - y; }";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::FunctionLiteral* fnLit = dynamic_cast<interp::ast::FunctionLiteral*>(expstmnt->expression.get()))
		{
			ASSERT_EQ(2, fnLit->params.size())
				<< "fn params does not contain 2 identifiers, got=" << std::to_string(fnLit->params.size());

			test_identifier(fnLit->params[0], "x");
			test_identifier(fnLit->params[1], "y");


			if (interp::ast::BlockExpression* fnBody = dynamic_cast<interp::ast::BlockExpression*>(fnLit->body.get()))
			{
				ASSERT_EQ(1, fnBody->statements.size())
					<< "consequence.statements does not contain 1 statement. got=" << std::to_string(fnBody->statements.size());

				if (interp::ast::ExpressionStatement* fnExpr = dynamic_cast<interp::ast::ExpressionStatement*>(fnBody->statements[0].get()))
				{
					if (interp::ast::InfixExpression* fnStmnt = dynamic_cast<interp::ast::InfixExpression*>(fnExpr->expression.get()))
					{
						test_identifier(fnStmnt->left, "x");

						EXPECT_EQ("-", fnStmnt->p_operator) << "operatior not - got " << fnStmnt->p_operator;

						test_identifier(fnStmnt->right, "y");
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
			else
			{
				EXPECT_TRUE(false) << "expression not BlockExpression";
			}
		}
		else
		{
			EXPECT_TRUE(false) << "expression not FunctionLiteral";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestFunctionLiteralTwoParams)
{
	std::string input = "fn(x, y) x - y";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::FunctionLiteral* fnLit = dynamic_cast<interp::ast::FunctionLiteral*>(expstmnt->expression.get()))
		{
			ASSERT_EQ(2, fnLit->params.size())
				<< "fn params does not contain 2 identifiers, got=" << std::to_string(fnLit->params.size());

			test_identifier(fnLit->params[0], "x");
			test_identifier(fnLit->params[1], "y");


			if (interp::ast::InfixExpression* fnStmnt = dynamic_cast<interp::ast::InfixExpression*>(fnLit->body.get()))
			{
				test_identifier(fnStmnt->left, "x");

				EXPECT_EQ("-", fnStmnt->p_operator) << "operatior not - got " << fnStmnt->p_operator;

				test_identifier(fnStmnt->right, "y");
			}
			else
			{
				EXPECT_TRUE(false) << "expression not InfixExpression";
			}
		}
		else
		{
			EXPECT_TRUE(false) << "expression not FunctionLiteral";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestFunctionLiteralNoParams)
{
	std::string input = "fn() x - y";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::FunctionLiteral* fnLit = dynamic_cast<interp::ast::FunctionLiteral*>(expstmnt->expression.get()))
		{
			ASSERT_EQ(0, fnLit->params.size())
				<< "fn params does not contain 2 identifiers, got=" << std::to_string(fnLit->params.size());


			if (interp::ast::InfixExpression* fnStmnt = dynamic_cast<interp::ast::InfixExpression*>(fnLit->body.get()))
			{
				test_identifier(fnStmnt->left, "x");

				EXPECT_EQ("-", fnStmnt->p_operator) << "operatior not - got " << fnStmnt->p_operator;

				test_identifier(fnStmnt->right, "y");
			}
			else
			{
				EXPECT_TRUE(false) << "expression not InfixExpression";
			}
		}
		else
		{
			EXPECT_TRUE(false) << "expression not FunctionLiteral";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestFunctionLiteralOneParam)
{
	std::string input = "fn(x) x - y";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::FunctionLiteral* fnLit = dynamic_cast<interp::ast::FunctionLiteral*>(expstmnt->expression.get()))
		{
			ASSERT_EQ(1, fnLit->params.size())
				<< "fn params does not contain 1 identifiers, got=" << std::to_string(fnLit->params.size());

			test_identifier(fnLit->params[0], "x");


			if (interp::ast::InfixExpression* fnStmnt = dynamic_cast<interp::ast::InfixExpression*>(fnLit->body.get()))
			{
				test_identifier(fnStmnt->left, "x");

				EXPECT_EQ("-", fnStmnt->p_operator) << "operatior not - got " << fnStmnt->p_operator;

				test_identifier(fnStmnt->right, "y");
			}
			else
			{
				EXPECT_TRUE(false) << "expression not InfixExpression";
			}
		}
		else
		{
			EXPECT_TRUE(false) << "expression not FunctionLiteral";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestCallTwoParams)
{
	std::string input = "adder(x, y)";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::CallExpression* callExpr = dynamic_cast<interp::ast::CallExpression*>(expstmnt->expression.get()))
		{
			test_identifier(callExpr->function, "adder");

			ASSERT_EQ(2, callExpr->args.size())
				<< "call args does not contain 2 identifiers, got=" << std::to_string(callExpr->args.size());

			test_identifier(callExpr->args[0], "x");
			test_identifier(callExpr->args[1], "y");
		}
		else
		{
			EXPECT_TRUE(false) << "expression not CallExpression";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestCallNoParams)
{
	std::string input = "adder()";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::CallExpression* callExpr = dynamic_cast<interp::ast::CallExpression*>(expstmnt->expression.get()))
		{
			test_identifier(callExpr->function, "adder");

			ASSERT_EQ(0, callExpr->args.size())
				<< "call args does not contain 0 identifiers, got=" << std::to_string(callExpr->args.size());
		}
		else
		{
			EXPECT_TRUE(false) << "expression not CallExpression";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
	}
}

TEST(ParserTest, TestCallOneParam)
{
	std::string input = "(fn(x) x-y)(x)";

	interp::lexer::Lexer lex(input);
	interp::parser::Parser parse(lex);

	auto prog = parse.parse_program();
	check_parser_errors(parse);

	ASSERT_EQ(1, prog->statements.size())
		<< "program.Statements does not contain 1 statements. got=" << std::to_string(prog->statements.size());

	if (interp::ast::ExpressionStatement* expstmnt = dynamic_cast<interp::ast::ExpressionStatement*>(prog->statements[0].get()))
	{
		if (interp::ast::CallExpression* callExpr = dynamic_cast<interp::ast::CallExpression*>(expstmnt->expression.get()))
		{
			if (interp::ast::FunctionLiteral* fnLit = dynamic_cast<interp::ast::FunctionLiteral*>(callExpr->function.get()))
			{
				ASSERT_EQ(1, fnLit->params.size())
					<< "fn params does not contain 1 identifiers, got=" << std::to_string(fnLit->params.size());

				test_identifier(fnLit->params[0], "x");


				if (interp::ast::InfixExpression* fnStmnt = dynamic_cast<interp::ast::InfixExpression*>(fnLit->body.get()))
				{
					test_identifier(fnStmnt->left, "x");

					EXPECT_EQ("-", fnStmnt->p_operator) << "operatior not - got " << fnStmnt->p_operator;

					test_identifier(fnStmnt->right, "y");
				}
				else
				{
					EXPECT_TRUE(false) << "expression not InfixExpression";
				}
			}
			else
			{
				EXPECT_TRUE(false) << "expression not FunctionLiteral";
			}

			ASSERT_EQ(1, callExpr->args.size())
				<< "call args does not contain 1 identifiers, got=" << std::to_string(callExpr->args.size());

			test_identifier(callExpr->args[0], "x");
		}
		else
		{
			EXPECT_TRUE(false) << "expression not CallExpression";
		}
	}
	else
	{
		EXPECT_TRUE(false) << "stmnt not ExpressionStatement";
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

void test_boolean(std::shared_ptr<interp::ast::Expression> expr, bool value)
{
	if (interp::ast::BooleanLiteral* boolean = dynamic_cast<interp::ast::BooleanLiteral*>(expr.get()))
	{
		EXPECT_EQ(value, boolean->value) << "value not " << value << " got " << boolean->value;
		EXPECT_EQ(value ? "true" : "false", boolean->token_literal()) 
			<< "token literal not " << (value ? "true" : "false") << " got " << boolean->token_literal();
	}
	else
	{
		EXPECT_TRUE(false) << "expression not Boolean";
	}
}
