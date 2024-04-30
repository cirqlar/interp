#include <iostream>
#include <string>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "repl.h"
#include "eval.h"

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
			auto errors = parse.get_errors();

			if (errors.size() > 0)
			{
				for (auto& error : parse.get_errors())
				{
					std::cout << '\t' << error << '\n';
				}
				input.clear();
				continue;
			}
			
			auto evaluated = interp::eval::eval(prog.get());
			if (evaluated)
			{
				std::cout << evaluated->inspect() << '\n';
			}

			input.clear();
		}
	}
}