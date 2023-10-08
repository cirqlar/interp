#include <iostream>
#include "shared/token.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

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

	std::string input2 = R"(
return 5;
return 10;
return 838383;
	)";

	interp::lexer::Lexer lex2(input2);
	interp::parser::Parser parse(lex2);

	auto prog = parse.parse_program();
	std::cout << prog->statements.size();
}