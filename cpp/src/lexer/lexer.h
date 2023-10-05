#ifndef INTERP_LEXER_H
#define INTERP_LEXER_H

#include <iostream>
#include "shared/token.h"

namespace interp::lexer
{
	class Lexer
	{
	public:
		Lexer(std::string input);
		~Lexer() = default;
		interp::shared::Token next_token();

	private:
		std::string input;
		int position;
		int read_position;
		char ch;

		void read_char();
		char peek_char();
		void skip_whitespace();
		std::string read_while(bool (*func)(char));
		interp::shared::Token new_token(interp::shared::TokenType Type, char ch);
	};
}

#endif