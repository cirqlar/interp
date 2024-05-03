#pragma once

#include <iostream>
#include "token.h"

namespace interp::lexer
{
	class Lexer
	{
	public:
		Lexer(std::string input);
		~Lexer() = default;
		interp::token::Token next_token();

	private:
		std::string input;
		size_t position;
		size_t read_position;
		char ch;

		void read_char();
		char peek_char();
		void skip_whitespace();
		std::string read_while(bool (*func)(char));
		std::string read_until(bool(*func)(char));
		interp::token::Token new_token(interp::token::TokenType Type, char in_ch);
	};
}
