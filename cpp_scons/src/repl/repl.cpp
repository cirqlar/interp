#include <iostream>
#include <string>

#include "lexer/token.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "repl.h"
#include "parser/eval.h"

namespace interp::repl
{
	void start()
	{

		auto env = interp::object::Environment::new_env(nullptr);

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
			
			auto evaluated = interp::eval::eval(prog, env);
			if (evaluated)
			{
				std::cout << evaluated->inspect() << '\n';
			}

			input.clear();
		}
	}
}