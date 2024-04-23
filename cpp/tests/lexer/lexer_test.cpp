#include <gtest/gtest.h>
#include "lexer.h"
#include "token.h"

// Demonstrate some basic assertions.
TEST(LexerTest, TestNextToken)
{
	std::string input = R"(let five = 5;
let ten = 10;
let add = fn(x, y) {
x + y;
};
let result = add(five, ten);
!-/*5;
5 < 10 > 5;
if (5 < 10) {
return true;
} else {
return false;
}
10 == 10;
10 != 9;
<=
>=
)";


	std::pair<interp::token::TokenType, std::string> expected[] = {
		std::pair(interp::token::LET, "let"),
		std::pair(interp::token::IDENT, "five"),
		std::pair(interp::token::ASSIGN, "="),
		std::pair(interp::token::INT, "5"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::LET, "let"),
		std::pair(interp::token::IDENT, "ten"),
		std::pair(interp::token::ASSIGN, "="),
		std::pair(interp::token::INT, "10"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::LET, "let"),
		std::pair(interp::token::IDENT, "add"),
		std::pair(interp::token::ASSIGN, "="),
		std::pair(interp::token::FUNCTION, "fn"),
		std::pair(interp::token::LPAREN, "("),
		std::pair(interp::token::IDENT, "x"),
		std::pair(interp::token::COMMA, ","),
		std::pair(interp::token::IDENT, "y"),
		std::pair(interp::token::RPAREN, ")"),
		std::pair(interp::token::LBRACE, "{"),
		std::pair(interp::token::IDENT, "x"),
		std::pair(interp::token::PLUS, "+"),
		std::pair(interp::token::IDENT, "y"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::RBRACE, "}"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::LET, "let"),
		std::pair(interp::token::IDENT, "result"),
		std::pair(interp::token::ASSIGN, "="),
		std::pair(interp::token::IDENT, "add"),
		std::pair(interp::token::LPAREN, "("),
		std::pair(interp::token::IDENT, "five"),
		std::pair(interp::token::COMMA, ","),
		std::pair(interp::token::IDENT, "ten"),
		std::pair(interp::token::RPAREN, ")"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::BANG, "!"),
		std::pair(interp::token::MINUS, "-"),
		std::pair(interp::token::FORWARDSLASH, "/"),
		std::pair(interp::token::ASTERISK, "*"),
		std::pair(interp::token::INT, "5"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::INT, "5"),
		std::pair(interp::token::LESSTHAN, "<"),
		std::pair(interp::token::INT, "10"),
		std::pair(interp::token::GREATERTHAN, ">"),
		std::pair(interp::token::INT, "5"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::IF, "if"),
		std::pair(interp::token::LPAREN, "("),
		std::pair(interp::token::INT, "5"),
		std::pair(interp::token::LESSTHAN, "<"),
		std::pair(interp::token::INT, "10"),
		std::pair(interp::token::RPAREN, ")"),
		std::pair(interp::token::LBRACE, "{"),
		std::pair(interp::token::RETURN, "return"),
		std::pair(interp::token::TRUE, "true"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::RBRACE, "}"),
		std::pair(interp::token::ELSE, "else"),
		std::pair(interp::token::LBRACE, "{"),
		std::pair(interp::token::RETURN, "return"),
		std::pair(interp::token::FALSE, "false"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::RBRACE, "}"),
		std::pair(interp::token::INT, "10"),
		std::pair(interp::token::EQUAL, "=="),
		std::pair(interp::token::INT, "10"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::INT, "10"),
		std::pair(interp::token::NOTEQUAL, "!="),
		std::pair(interp::token::INT, "9"),
		std::pair(interp::token::SEMICOLON, ";"),
		std::pair(interp::token::LESSTHANOREQUAL, "<="),
		std::pair(interp::token::GREATERTHANOREQUAL, ">="),
		std::pair(interp::token::L_EOF, ""),
	};


	interp::lexer::Lexer lex = interp::lexer::Lexer(input);


	for (auto tpair : expected)
	{
		auto tok = lex.next_token();
		
		EXPECT_EQ(tpair.first, tok.type);
		EXPECT_EQ(tpair.second, tok.literal);
	}


	// // Expect two strings not to be equal.
	// EXPECT_STRNE("hello", "world");
	// // Expect equality.
	// EXPECT_EQ(7 * 6, 42);
}