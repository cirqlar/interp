#include "parser.h"

namespace interp::parser
{
	Parser::Parser(interp::lexer::Lexer lexer) : lexer(lexer)
	{
		this->next_token();
		this->next_token();
	}

	std::shared_ptr<interp::ast::Program> Parser::parse_program()
	{
		auto prog = std::shared_ptr<interp::ast::Program>(new interp::ast::Program());

		while (!this->current_token_is(interp::token::L_EOF))
		{
			auto stmnt = this->parse_statement();
			if (stmnt)
			{
				prog->statements.push_back(stmnt);
			}
			this->next_token();
		}

		return prog;
	}

	std::vector<std::string> Parser::get_errors()
	{
		return this->errors;
	}

	void Parser::next_token()
	{
		this->current_token = this->peek_token;
		this->peek_token = this->lexer.next_token();
	}

	std::shared_ptr<interp::ast::Statement> Parser::parse_statement()
	{
		if (this->current_token.type == interp::token::LET)
		{
			return this->parse_let_statement();
		}
		else if (this->current_token.type == interp::token::RETURN)
		{
			return this->parse_return_statement();
		}
		else
		{
			return std::shared_ptr<interp::ast::Statement>(nullptr);
		}
	}

	std::shared_ptr<interp::ast::LetStatement> Parser::parse_let_statement()
	{
		auto current = this->current_token;

		if (!this->expect_peek(interp::token::IDENT))
		{
			return std::shared_ptr<interp::ast::LetStatement>(nullptr);
		}
		auto name = interp::ast::Identifier(this->current_token, this->current_token.literal);

		if (!this->expect_peek(interp::token::ASSIGN))
		{
			return std::shared_ptr<interp::ast::LetStatement>(nullptr);
		}

		// Skip to semicolon
		while (!this->current_token_is(interp::token::SEMICOLON))
		{
			this->next_token();
		}

		return std::shared_ptr<interp::ast::LetStatement>(
			new interp::ast::LetStatement(current, name, std::shared_ptr<interp::ast::Expression>(nullptr)));
	}

	std::shared_ptr<interp::ast::ReturnStatement> Parser::parse_return_statement()
	{
		auto current = this->current_token;

		// Skip to semicolon
		while (!this->current_token_is(interp::token::SEMICOLON))
		{
			this->next_token();
		}

		return std::shared_ptr<interp::ast::ReturnStatement>(
			new interp::ast::ReturnStatement(current, std::shared_ptr<interp::ast::Expression>(nullptr)));
	}

	bool Parser::current_token_is(interp::token::TokenType type)
	{
		return this->current_token.type == type;
	}

	bool Parser::peek_token_is(interp::token::TokenType type)
	{
		return this->peek_token.type == type;
	}

	bool Parser::expect_peek(interp::token::TokenType type)
	{
		if (this->peek_token_is(type))
		{
			this->next_token();
			return true;
		}
		else
		{
			this->peek_error(type);
			return false;
		}
	}

	void Parser::peek_error(interp::token::TokenType type)
	{
		this->errors.push_back("Expected next token to be " + type + ", got " + this->peek_token.type + " instead");
	}
}