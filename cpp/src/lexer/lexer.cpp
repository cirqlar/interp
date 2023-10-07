#include "lexer.h"

bool isLetter(char ch)
{
	return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_';
}

bool isDigit(char ch)
{
	return ch >= '0' && ch <= '9';
}

namespace interp::lexer
{

	Lexer::Lexer(std::string input)
	{
		this->input = input;
		this->read_char();
	}

	interp::token::Token Lexer::next_token()
	{
		interp::token::Token tok;

		this->skip_whitespace();

		switch (this->ch)
		{
		case '=':
			if (this->peek_char() == '=')
			{
				char ch = this->ch;
				this->read_char();
				tok = {.type = interp::token::EQUAL, .literal = std::string(1, ch) + std::string(1, this->ch)};
			}
			else
			{
				tok = this->new_token(interp::token::ASSIGN, this->ch);
			}
			break;
		case '+':
			tok = this->new_token(interp::token::PLUS, this->ch);
			break;
		case '-':
			tok = this->new_token(interp::token::MINUS, this->ch);
			break;
		case '*':
			tok = this->new_token(interp::token::ASTERISK, this->ch);
			break;
		case '/':
			tok = this->new_token(interp::token::FORWARDSLASH, this->ch);
			break;
		case '!':
			if (this->peek_char() == '=')
			{
				char ch = this->ch;
				this->read_char();
				tok = {.type = interp::token::NOTEQUAL, .literal = std::string(1, ch) + std::string(1, this->ch)};
			}
			else
			{
				tok = this->new_token(interp::token::BANG, this->ch);
			}
			break;
		case '<':
			if (this->peek_char() == '=')
			{
				char ch = this->ch;
				this->read_char();
				tok = {.type = interp::token::LESSTHANOREQUAL, .literal = std::string(1, ch) + std::string(1, this->ch)};
			}
			else
			{
				tok = this->new_token(interp::token::LESSTHAN, this->ch);
			}
			break;
		case '>':
			if (this->peek_char() == '=')
			{
				char ch = this->ch;
				this->read_char();
				tok = {.type = interp::token::GREATERTHANOREQUAL, .literal = std::string(1, ch) + std::string(1, this->ch)};
			}
			else
			{
				tok = this->new_token(interp::token::GREATERTHAN, this->ch);
			}
			break;
		case ',':
			tok = this->new_token(interp::token::COMMA, this->ch);
			break;
		case ';':
			tok = this->new_token(interp::token::SEMICOLON, this->ch);
			break;
		case '(':
			tok = this->new_token(interp::token::LPAREN, this->ch);
			break;
		case ')':
			tok = this->new_token(interp::token::RPAREN, this->ch);
			break;
		case '{':
			tok = this->new_token(interp::token::LBRACE, this->ch);
			break;
		case '}':
			tok = this->new_token(interp::token::RBRACE, this->ch);
			break;
		case 0:
			tok = {.type = interp::token::L_EOF, .literal = ""};
			break;
		default:
			if (isLetter(this->ch))
			{
				std::string literal = this->read_while(&isLetter);
				return {.type = interp::token::lookup_ident(literal), .literal = literal};
			}
			else if (isDigit(this->ch))
			{
				return {.type = interp::token::INT, .literal = this->read_while(&isDigit)};
			}
			else
			{
				return this->new_token(interp::token::ILLEGAL, this->ch);
			}
			break;
		}

		this->read_char();
		return tok;
	}

	void Lexer::read_char()
	{
		if (this->read_position >= this->input.length())
		{
			this->ch = 0;
		}
		else
		{
			this->ch = this->input.at(this->read_position);
		}

		this->position = this->read_position;
		this->read_position += 1;
	}

	char Lexer::peek_char()
	{
		if (this->read_position >= this->input.length())
		{
			return 0;
		}
		else
		{
			return this->input.at(this->read_position);
		}
	}

	void Lexer::skip_whitespace()
	{
		while (this->ch == ' ' || this->ch == '\n' || this->ch == '\t' || this->ch == '\r')
		{
			this->read_char();
		}
	}

	std::string Lexer::read_while(bool (*func)(char))
	{
		int position = this->position;
		while (func(this->ch))
		{
			this->read_char();
		}
		return this->input.substr(position, this->position - position);
	}

	interp::token::Token Lexer::new_token(interp::token::TokenType Type, char ch)
	{
		return {.type = Type, .literal = std::string(1, ch)};
	}
}