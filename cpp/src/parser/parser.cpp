#include "parser.h"

namespace interp::parser
{
	Parser::Parser(interp::lexer::Lexer lexer) : lexer(lexer), prefix_parse_fns({}), infix_parse_fns({})
	{
		this->next_token();
		this->next_token();

		this->prefix_parse_fns[interp::token::IDENT] = this->parse_identifier;
		this->prefix_parse_fns[interp::token::INT] = this->parse_integer_literal;
		this->prefix_parse_fns[interp::token::BANG] = this->parse_prefix_expression;
		this->prefix_parse_fns[interp::token::MINUS] = this->parse_prefix_expression;
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
			return this->parse_expression_statement();
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

	std::shared_ptr<interp::ast::ExpressionStatement> Parser::parse_expression_statement()
	{
		auto exprstmnt = std::shared_ptr<interp::ast::ExpressionStatement>(
			new interp::ast::ExpressionStatement(this->current_token, this->parse_expression(LOWEST)));

		if (this->peek_token_is(interp::token::SEMICOLON))
		{
			this->next_token();
		}

		return exprstmnt;
	}

	std::shared_ptr<interp::ast::Expression> Parser::parse_expression(Precidence)
	{
		if (this->prefix_parse_fns.find(this->current_token.type) == this->prefix_parse_fns.end())
		{
			this->no_prefix_parse_fn_error(this->current_token.type);
			return std::shared_ptr<interp::ast::Expression>(nullptr);
		}
		auto prefix = this->prefix_parse_fns[this->current_token.type];

		auto left_expr = prefix(this);

		return left_expr;
	}

	std::shared_ptr<interp::ast::Expression> Parser::parse_identifier(Parser *p)
	{
		return std::shared_ptr<interp::ast::Identifier>(
			new interp::ast::Identifier(p->current_token, p->current_token.literal));
	}

	std::shared_ptr<interp::ast::Expression> Parser::parse_integer_literal(Parser *p)
	{
		try
		{
			auto val = std::stoll(p->current_token.literal);
			return std::shared_ptr<interp::ast::IntegerLiteral>(
				new interp::ast::IntegerLiteral(p->current_token, val));
		}
		catch (...)
		{
			p->errors.push_back("could not parse " + p->current_token.literal + " as an integer");
			return std::shared_ptr<interp::ast::IntegerLiteral>(nullptr);
		}
		return std::shared_ptr<interp::ast::IntegerLiteral>();
	}

	std::shared_ptr<interp::ast::Expression> Parser::parse_prefix_expression(Parser *p)
	{
		auto current_token = p->current_token;
		p->next_token();
		return std::shared_ptr<interp::ast::PrefixExpression>(
			new interp::ast::PrefixExpression(
				current_token,
				current_token.literal,
				p->parse_expression(PREFIX)
			)
		);
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

	void Parser::no_prefix_parse_fn_error(interp::token::TokenType type)
	{
		this->errors.push_back("No prefix parse fn found for " + type);
	}
}