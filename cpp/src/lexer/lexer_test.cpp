#include <gtest/gtest.h>
#include "lexer.h"
#include "shared/token.h"

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


	std::pair<interp::shared::TokenType, std::string> expected[] = {
		std::pair(interp::shared::LET, "let"),
		std::pair(interp::shared::IDENT, "five"),
		std::pair(interp::shared::ASSIGN, "="),
		std::pair(interp::shared::INT, "5"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::LET, "let"),
		std::pair(interp::shared::IDENT, "ten"),
		std::pair(interp::shared::ASSIGN, "="),
		std::pair(interp::shared::INT, "10"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::LET, "let"),
		std::pair(interp::shared::IDENT, "add"),
		std::pair(interp::shared::ASSIGN, "="),
		std::pair(interp::shared::FUNCTION, "fn"),
		std::pair(interp::shared::LPAREN, "("),
		std::pair(interp::shared::IDENT, "x"),
		std::pair(interp::shared::COMMA, ","),
		std::pair(interp::shared::IDENT, "y"),
		std::pair(interp::shared::RPAREN, ")"),
		std::pair(interp::shared::LBRACE, "{"),
		std::pair(interp::shared::IDENT, "x"),
		std::pair(interp::shared::PLUS, "+"),
		std::pair(interp::shared::IDENT, "y"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::RBRACE, "}"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::LET, "let"),
		std::pair(interp::shared::IDENT, "result"),
		std::pair(interp::shared::ASSIGN, "="),
		std::pair(interp::shared::IDENT, "add"),
		std::pair(interp::shared::LPAREN, "("),
		std::pair(interp::shared::IDENT, "five"),
		std::pair(interp::shared::COMMA, ","),
		std::pair(interp::shared::IDENT, "ten"),
		std::pair(interp::shared::RPAREN, ")"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::BANG, "!"),
		std::pair(interp::shared::MINUS, "-"),
		std::pair(interp::shared::FORWARDSLASH, "/"),
		std::pair(interp::shared::ASTERISK, "*"),
		std::pair(interp::shared::INT, "5"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::INT, "5"),
		std::pair(interp::shared::LESSTHAN, "<"),
		std::pair(interp::shared::INT, "10"),
		std::pair(interp::shared::GREATERTHAN, ">"),
		std::pair(interp::shared::INT, "5"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::IF, "if"),
		std::pair(interp::shared::LPAREN, "("),
		std::pair(interp::shared::INT, "5"),
		std::pair(interp::shared::LESSTHAN, "<"),
		std::pair(interp::shared::INT, "10"),
		std::pair(interp::shared::RPAREN, ")"),
		std::pair(interp::shared::LBRACE, "{"),
		std::pair(interp::shared::RETURN, "return"),
		std::pair(interp::shared::TRUE, "true"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::RBRACE, "}"),
		std::pair(interp::shared::ELSE, "else"),
		std::pair(interp::shared::LBRACE, "{"),
		std::pair(interp::shared::RETURN, "return"),
		std::pair(interp::shared::FALSE, "false"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::RBRACE, "}"),
		std::pair(interp::shared::INT, "10"),
		std::pair(interp::shared::EQUAL, "=="),
		std::pair(interp::shared::INT, "10"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::INT, "10"),
		std::pair(interp::shared::NOTEQUAL, "!="),
		std::pair(interp::shared::INT, "9"),
		std::pair(interp::shared::SEMICOLON, ";"),
		std::pair(interp::shared::LESSTHANOREQUAL, "<="),
		std::pair(interp::shared::GREATERTHANOREQUAL, ">="),
		std::pair(interp::shared::L_EOF, ""),
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