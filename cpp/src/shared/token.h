#ifndef INTERP_SHARED_TOKEN_H
#define INTERP_SHARED_TOKEN_H

#include <iostream>
#include <map>

namespace interp::shared
{
	typedef std::string TokenType;

	struct Token
	{
		TokenType type;
		std::string literal;
	};

	const std::string
		ILLEGAL = "ILLEGAL",
		L_EOF = "EOF",

		// Identifiers + literals
		IDENT = "IDENT", // add, foobar, x, y, ...
		INT = "INT",	 // 1343456

		// Operators
		ASSIGN = "=",
		EQUAL = "==",
		PLUS = "+",
		MINUS = "-",
		ASTERISK = "*",
		FORWARDSLASH = "/",
		BANG = "!",
		NOTEQUAL = "!=",
		LESSTHAN = "<",
		LESSTHANOREQUAL = "<=",
		GREATERTHAN = ">",
		GREATERTHANOREQUAL = ">=",

		// Delimiters
		COMMA = ",",
		SEMICOLON = ";",

		LPAREN = "(",
		RPAREN = ")",
		LBRACE = "{",
		RBRACE = "}",

		// Keywords
		FUNCTION = "FUNCTION",
		LET = "LET",
		TRUE = "TRUE",
		FALSE = "FALSE",
		IF = "IF",
		ELSE = "ELSE",
		RETURN = "RETURN";

	std::string lookup_ident(std::string literal);
}

#endif