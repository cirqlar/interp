#include <gtest/gtest.h>

#include "shared/token.h"
#include "ast.h"

TEST(AstTest, TestString)
{
	interp::ast::Program prog = interp::ast::Program(std::vector({ 
		std::shared_ptr<interp::ast::Statement>(new interp::ast::LetStatement(
			{.type = interp::token::LET, .literal = "let"},
			interp::ast::Identifier({.type = interp::token::IDENT, .literal = "myVar"}, "myVar"),
			std::shared_ptr<interp::ast::Expression>(
				new interp::ast::Identifier({.type = interp::token::IDENT, .literal = "anotherVar"}, "anotherVar")
			)
		))
	}));
	EXPECT_EQ("let myVar = anotherVar;", prog.string());
}