#include <gtest/gtest.h>

#include "parser.h"

void check_parser_errors(interp::parser::Parser p);
void test_let_statment(std::shared_ptr<interp::ast::Statement> stmnt, std::string name);

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
		<< "program.Statements does not contain 3 statements. got=" + std::to_string(prog->statements.size());

	std::vector<std::string> expected = {"x", "y", "foobar"};

	for (int i = 0; i < expected.size(); i++)
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
		<< "program.Statements does not contain 3 statements. got=" + std::to_string(prog->statements.size());

	for (auto stmnt : prog->statements)
	{
		if (interp::ast::ReturnStatement * retstmnt = dynamic_cast<interp::ast::ReturnStatement *>(stmnt.get()))
		{
			EXPECT_EQ("return", retstmnt->token_literal()) << "token literal not return got " + retstmnt->token_literal();
		}
		else {
			EXPECT_TRUE(false) << "stmnt not ReturnStatement";
		}
	}
}

void check_parser_errors(interp::parser::Parser p)
{
	auto errors = p.get_errors();
	if (errors.size() == 0)
	{
		return;
	}

	EXPECT_TRUE(false) << "parser has " + std::to_string(errors.size()) + " errors";
	for (auto error : errors)
	{
		EXPECT_TRUE(false) << "parser error: " << error;
	}
	ASSERT_TRUE(false);
}

void test_let_statment(std::shared_ptr<interp::ast::Statement> stmnt, std::string name)
{
	ASSERT_EQ("let", stmnt->token_literal()) << "stmnt literal not 'let'. got=" + stmnt->token_literal();

	if (interp::ast::LetStatement *letstmnt = dynamic_cast<interp::ast::LetStatement *>(stmnt.get()))
	{
		ASSERT_EQ(name, letstmnt->name.value) << "name not " + name + " got " + letstmnt->name.value;
		ASSERT_EQ(name, letstmnt->name.token_literal()) << "name.token_literal not " + name + " got " + letstmnt->name.token_literal();
	}
	else
	{
		ASSERT_TRUE(false) << "stmnt not LetStatement";
	}
}
