#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "token.h"
#include "lexer.h"
#include "ast.h"

namespace interp::parser
{
	enum Precidence
	{
		LOWEST,
		EQUALS,		 // ==
		LESSGREATER, // > or < or <= or >=
		SUM,		 // +
		PRODUCT,	 // *
		PREFIX,		 // -X or !X
		CALL,		 // myFunction(X)
	};

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

		std::map<interp::token::TokenType, std::shared_ptr<interp::ast::Expression> (*)(Parser *)> prefix_parse_fns;
		std::map<interp::token::TokenType, std::shared_ptr<interp::ast::Expression> (*)(Parser *, std::shared_ptr<interp::ast::Expression>)> infix_parse_fns;

		void next_token();
		std::shared_ptr<interp::ast::Statement> parse_statement();
		std::shared_ptr<interp::ast::LetStatement> parse_let_statement();
		std::shared_ptr<interp::ast::ReturnStatement> parse_return_statement();
		std::shared_ptr<interp::ast::ExpressionStatement> parse_expression_statement();
		std::shared_ptr<interp::ast::Expression> parse_expression(Precidence);
		static std::shared_ptr<interp::ast::Expression> parse_identifier(Parser *);
		static std::shared_ptr<interp::ast::Expression> parse_integer_literal(Parser *);
		static std::shared_ptr<interp::ast::Expression> parse_boolean(Parser*);
		static std::shared_ptr<interp::ast::Expression> parse_grouped_expression(Parser*);
		static std::shared_ptr<interp::ast::Expression> parse_if_expression(Parser*);
		static std::shared_ptr<interp::ast::Expression> parse_block_expression(Parser *);
		static std::shared_ptr<interp::ast::Expression> parse_prefix_expression(Parser *);
		static std::shared_ptr<interp::ast::Expression> parse_infix_expression(Parser *, std::shared_ptr<interp::ast::Expression> left);

		bool current_token_is(interp::token::TokenType type);
		bool peek_token_is(interp::token::TokenType type);
		bool expect_peek(interp::token::TokenType type);
		void peek_error(interp::token::TokenType type);
		void no_prefix_parse_fn_error(interp::token::TokenType type);
		Precidence peek_precidence();
		Precidence curr_precidence();
	};
}
