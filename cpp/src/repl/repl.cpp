#include <iostream>
#include <string>

#include "token.h"
#include "lexer.h"
#include "parser.h"
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
			interp::parser::Parser parse(lex);

			auto prog = parse.parse_program();

			for (auto& error : parse.get_errors())
			{
				std::cout << '\t' << error << '\n';
				goto end_while_loop;
			}

			std::cout << prog->string() << '\n';

			end_while_loop:
			input.clear();
		}
	}
}