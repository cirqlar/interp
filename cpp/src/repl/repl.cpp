#include <iostream>
#include <string>

#include "shared/token.h"
#include "lexer/lexer.h"
#include "repl.h"

namespace interp::repl
{
	void start()
	{

		while (true) {
			std::string input;
			std::cout << ">> ";
			std::getline(std::cin, input);

			interp::lexer::Lexer lex(input);
			for (auto tok = lex.next_token(); tok.type != interp::token::L_EOF; tok = lex.next_token())
			{
				std::cout << "Type: " << tok.type << " Literal: " << tok.literal << '\n';
			}
			input.clear();
		}
	}
}