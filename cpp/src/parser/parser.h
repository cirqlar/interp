#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "shared/token.h"
#include "lexer/lexer.h"
#include "shared/ast.h"

namespace interp::parser
{
	class Parser
	{
	public:
		Parser(interp::lexer::Lexer lexer);
		~Parser() = default;

		std::shared_ptr<interp::ast::Program> parse_program();
		std::vector<std::string> get_errors();

	private:
		interp::lexer::Lexer lexer;
		interp::token::Token current_token;
		interp::token::Token peek_token;
		std::vector<std::string> errors;

		void next_token();
		std::shared_ptr<interp::ast::Statement> parse_statement();
		std::shared_ptr<interp::ast::LetStatement> parse_let_statement();
		std::shared_ptr<interp::ast::ReturnStatement> parse_return_statement();
		bool current_token_is(interp::token::TokenType type);
		bool peek_token_is(interp::token::TokenType type);
		bool expect_peek(interp::token::TokenType type);
		void peek_error(interp::token::TokenType type);
	};
}
