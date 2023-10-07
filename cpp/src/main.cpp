#include <iostream>
#include "shared/token.h"
#include "lexer/lexer.h"

int main()
{
	std::string input = R"(let five = 5;
let ten = 10;
let add = fn(x, y) {
x + y;
};
let result = add(five, ten);
	)";

	interp::lexer::Lexer lex(input);
	interp::token::Token tok;

	do {
		tok = lex.next_token();
		std::cout << "Token {.type = " << tok.type << ", .literal = " << tok.literal << "}\n";
	} while (tok.type != interp::token::L_EOF && tok.type != interp::token::ILLEGAL);
}