#include "parser.h"

namespace interp::parser
{
	std::map<token::TokenType, Precidence> precidences({
		std::pair(token::EQUAL, Precidence::EQUALS),
		std::pair(token::NOTEQUAL, Precidence::EQUALS),
		std::pair(token::LESSTHANOREQUAL, Precidence::LESSGREATER),
		std::pair(token::GREATERTHANOREQUAL, Precidence::LESSGREATER),
		std::pair(token::LESSTHAN, Precidence::LESSGREATER),
		std::pair(token::GREATERTHAN, Precidence::LESSGREATER),
		std::pair(token::PLUS, Precidence::SUM),
		std::pair(token::MINUS, Precidence::SUM),
		std::pair(token::FORWARDSLASH, Precidence::PRODUCT),
		std::pair(token::ASTERISK, Precidence::PRODUCT),
	});

	Parser::Parser(interp::lexer::Lexer lexer) : lexer(lexer), prefix_parse_fns({}), infix_parse_fns({})
	{
		this->next_token();
		this->next_token();

		this->prefix_parse_fns[interp::token::IDENT] = this->parse_identifier;
		this->prefix_parse_fns[interp::token::INT] = this->parse_integer_literal;
		this->prefix_parse_fns[interp::token::BANG] = this->parse_prefix_expression;
		this->prefix_parse_fns[interp::token::MINUS] = this->parse_prefix_expression;
		this->prefix_parse_fns[interp::token::TRUE] = this->parse_boolean;
		this->prefix_parse_fns[interp::token::FALSE] = this->parse_boolean;
		this->prefix_parse_fns[interp::token::LPAREN] = this->parse_grouped_expression;
		this->prefix_parse_fns[interp::token::IF] = this->parse_if_expression;
		this->prefix_parse_fns[interp::token::LBRACE] = this->parse_block_expression;
		this->prefix_parse_fns[interp::token::FUNCTION] = this->parse_function_literal;

		this->infix_parse_fns[interp::token::EQUAL] = this->parse_infix_expression;
		this->infix_parse_fns[interp::token::NOTEQUAL] = this->parse_infix_expression;
		this->infix_parse_fns[interp::token::LESSTHANOREQUAL] = this->parse_infix_expression;
		this->infix_parse_fns[interp::token::GREATERTHANOREQUAL] = this->parse_infix_expression;
		this->infix_parse_fns[interp::token::LESSTHAN] = this->parse_infix_expression;
		this->infix_parse_fns[interp::token::GREATERTHAN] = this->parse_infix_expression;
		this->infix_parse_fns[interp::token::PLUS] = this->parse_infix_expression;
		this->infix_parse_fns[interp::token::MINUS] = this->parse_infix_expression;
		this->infix_parse_fns[interp::token::FORWARDSLASH] = this->parse_infix_expression;
		this->infix_parse_fns[interp::token::ASTERISK] = this->parse_infix_expression;
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

	std::shared_ptr<interp::ast::Expression> Parser::parse_expression(Precidence in_precidence)
	{
		if (this->prefix_parse_fns.find(this->current_token.type) == this->prefix_parse_fns.end())
		{
			this->no_prefix_parse_fn_error(this->current_token.type);
			return nullptr;
			//return std::shared_ptr<interp::ast::Expression>(nullptr);
		}
		auto left_expr = this->prefix_parse_fns[this->current_token.type](this);

		while (!this->peek_token_is(interp::token::SEMICOLON) && in_precidence < this->peek_precidence())
		{
			if (this->infix_parse_fns.find(this->peek_token.type) == this->infix_parse_fns.end())
			{
				return left_expr;
			}
			this->next_token();

			left_expr = this->infix_parse_fns[this->current_token.type](this, left_expr);
		}

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
			auto val = std::atoll(p->current_token.literal.c_str());
			return std::shared_ptr<interp::ast::IntegerLiteral>(
				new interp::ast::IntegerLiteral(p->current_token, val));
		}
		catch (...)
		{
			p->errors.push_back("could not parse " + p->current_token.literal + " as an integer");
			return std::shared_ptr<interp::ast::IntegerLiteral>(nullptr);
		}
	}

	std::shared_ptr<interp::ast::Expression> Parser::parse_boolean(Parser *p)
	{
		return std::shared_ptr<interp::ast::Boolean>(
			new interp::ast::Boolean(p->current_token, p->current_token_is(interp::token::TRUE)));
	}

	std::shared_ptr<interp::ast::Expression> Parser::parse_grouped_expression(Parser *p)
	{
		p->next_token();

		auto expr = p->parse_expression(Precidence::LOWEST);

		if (!p->expect_peek(interp::token::RPAREN))
		{
			return nullptr;
		}

		return expr;
	}

	std::shared_ptr<interp::ast::Expression> Parser::parse_if_expression(Parser* p)
	{
		auto current_token = p->current_token;

		if (!p->expect_peek(interp::token::LPAREN))
		{
			return nullptr;
		}

		p->next_token();
		auto condition = p->parse_expression(Precidence::LOWEST);

		if (!p->expect_peek(interp::token::RPAREN))
		{
			return nullptr;
		}

		p->next_token();
		auto consequence = p->parse_expression(Precidence::LOWEST);

		std::shared_ptr<interp::ast::IfExpression> if_expr = std::shared_ptr<interp::ast::IfExpression>(
			new interp::ast::IfExpression(current_token, condition, consequence)
		);

		if (p->peek_token_is(interp::token::ELSE))
		{
			p->next_token();
			p->next_token();

			if_expr->alternative = p->parse_expression(Precidence::LOWEST);
		}

		return if_expr;
	}

	std::shared_ptr<interp::ast::Expression> Parser::parse_block_expression(Parser* p)
	{
		auto block = std::shared_ptr<interp::ast::BlockExpression>(new interp::ast::BlockExpression(p->current_token));

		p->next_token();

		while (!p->current_token_is(interp::token::L_EOF) && !p->current_token_is(interp::token::RBRACE))
		{
			auto stmnt = p->parse_statement();
			if (stmnt)
			{
				block->statements.push_back(stmnt);
			}
			p->next_token();
		}

		return block;
	}

	std::shared_ptr<interp::ast::Expression> Parser::parse_function_literal(Parser *p)
	{
		auto current_token = p->current_token;

		if (!p->expect_peek(interp::token::LPAREN))
		{
			return nullptr;
		}

		auto if_lit = std::shared_ptr<interp::ast::FunctionLiteral>(
			new interp::ast::FunctionLiteral(current_token)
		);
		p->parse_function_parameters(if_lit->params);

		p->next_token();

		if_lit->body = p->parse_expression(Precidence::LOWEST);
		
		return if_lit;
	}

	void Parser::parse_function_parameters(std::vector<std::shared_ptr<interp::ast::Identifier>>& out_params)
	{
		if (this->peek_token_is(interp::token::RPAREN))
		{
			this->next_token();
			return;
		}

		do
		{
			this->next_token();
			out_params.push_back(std::shared_ptr<interp::ast::Identifier>(
				new interp::ast::Identifier(this->current_token, this->current_token.literal)
			));
			this->next_token();
		} while (this->current_token_is(interp::token::COMMA));

		if (!this->current_token_is(interp::token::RPAREN))
		{
			this->current_error(interp::token::RPAREN);
		}
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

	std::shared_ptr<interp::ast::Expression> Parser::parse_infix_expression(Parser *p, std::shared_ptr<interp::ast::Expression> left)
	{
		auto current_token = p->current_token;
		auto current_precidence = p->curr_precidence();
		p->next_token();
		return std::shared_ptr<interp::ast::InfixExpression>(
			new interp::ast::InfixExpression(
				current_token,
				left,
				current_token.literal,
				p->parse_expression(current_precidence)
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

	void Parser::current_error(interp::token::TokenType type)
	{
		this->errors.push_back("Expected current token to be " + type + ", got " + this->current_token.type + " instead");
	}

	void Parser::peek_error(interp::token::TokenType type)
	{
		this->errors.push_back("Expected next token to be " + type + ", got " + this->peek_token.type + " instead");
	}

	void Parser::no_prefix_parse_fn_error(interp::token::TokenType type)
	{
		this->errors.push_back("No prefix parse fn found for " + type);
	}

	Precidence Parser::peek_precidence()
	{
		if (precidences.find(this->peek_token.type) == precidences.end())
		{
			return Precidence::LOWEST;
		}
		else {
			return precidences[this->peek_token.type];
		}
	}

	Precidence Parser::curr_precidence()
	{
		if (precidences.find(this->current_token.type) == precidences.end())
		{
			return Precidence::LOWEST;
		}
		else {
			return precidences[this->current_token.type];
		}
	}

}