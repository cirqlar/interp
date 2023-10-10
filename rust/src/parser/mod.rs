use std::collections::HashMap;

use crate::{
    lexer::Lexer,
    shared::ast::{Expression, Identifier, IntegerLiteral, PrefixExpression, Program, Statement},
    Token, TokenType,
};

enum Precidence {
    Lowest,
    Equals,      // ==
    Lessgreater, // > or < or <= or >=
    Sum,         // +
    Product,     // *
    Prefix,      // -X or !X
    Call,        // myFunction(X)
}

type PrefixParseFn = fn(&mut Parser) -> Result<Expression, String>;
type InfixParseFn = fn(&Expression) -> Result<Expression, String>;

pub struct Parser {
    l: Lexer,
    current_token: Token,
    peek_token: Token,
    pub errors: Vec<String>,

    prefix_parse_fns: HashMap<TokenType, PrefixParseFn>,
    infix_parse_fns: HashMap<TokenType, InfixParseFn>,
}

impl Parser {
    pub fn new(l: Lexer) -> Parser {
        let mut p = Parser {
            l,
            current_token: Token::ILLEGAL.clone(),
            peek_token: Token::ILLEGAL.clone(),
            errors: Vec::new(),
            prefix_parse_fns: HashMap::new(),
            infix_parse_fns: HashMap::new(),
        };

        p.next_token();
        p.next_token();

        p.prefix_parse_fns
            .insert(TokenType::Identifier, Parser::parse_identifier);
        p.prefix_parse_fns
            .insert(TokenType::Integer, Parser::parse_integer_literal);
        p.prefix_parse_fns
            .insert(TokenType::Bang, Parser::parse_prefix_expression);
        p.prefix_parse_fns
            .insert(TokenType::Minus, Parser::parse_prefix_expression);

        p
    }

    fn next_token(&mut self) {
        self.current_token = std::mem::replace(&mut self.peek_token, self.l.next_token());
    }

    pub fn parse_program(&mut self) -> Program {
        let mut prog = Program::new();

        while self.current_token != Token::EOF {
            match self.parse_statement() {
                Ok(stmnt) => prog.statements.push(stmnt),
                Err(e) => self.errors.push(e),
            }
            self.next_token();
        }

        prog
    }

    fn parse_statement(&mut self) -> Result<Statement, String> {
        match self.current_token.token_type {
            TokenType::Let => self.parse_let_statement(),
            TokenType::Return => self.parse_return_statement(),
            _ => self.parse_expression_statment(),
        }
    }

    fn parse_let_statement(&mut self) -> Result<Statement, String> {
        let current_token = std::mem::replace(&mut self.current_token, Token::ILLEGAL.clone());

        // let peek_token = ;
        self.expect_peek(&TokenType::Identifier)?;

        let name = Identifier {
            token: std::mem::replace(&mut self.current_token, Token::ILLEGAL.clone()),
        };

        self.expect_peek(&TokenType::Assign)?;

        // Skip till next semicolon
        while !self.current_token_is(&TokenType::Semicolon) {
            self.next_token();
        }

        Ok(Statement::Let {
            token: current_token,
            name,
            value: None,
        })
    }

    fn parse_return_statement(&mut self) -> Result<Statement, String> {
        let stmnt = Statement::Return {
            token: std::mem::replace(&mut self.current_token, Token::ILLEGAL.clone()),
            value: None,
        };
        self.next_token();

        // Skip till next semicolon
        while !self.current_token_is(&TokenType::Semicolon) {
            self.next_token();
        }

        Ok(stmnt)
    }

    fn parse_expression_statment(&mut self) -> Result<Statement, String> {
        let token = self.current_token.clone();

        let value = self.parse_expression(Precidence::Lowest)?;

        if self.peek_token_is(&TokenType::Semicolon) {
            self.next_token()
        }

        Ok(Statement::Expression { token, value })
    }

    fn parse_expression(&mut self, precidence: Precidence) -> Result<Expression, String> {
        let prefix = self
            .prefix_parse_fns
            .get(&self.current_token.token_type)
            .ok_or(format!(
                "No prefix found for {:?}",
                self.current_token.token_type
            ))?;

        prefix(self)
    }

    fn parse_identifier(&mut self) -> Result<Expression, String> {
        Ok(Identifier {
            token: std::mem::replace(&mut self.current_token, Token::ILLEGAL.clone()),
        }
        .into())
    }

    fn parse_integer_literal(&mut self) -> Result<Expression, String> {
        let token = std::mem::replace(&mut self.current_token, Token::ILLEGAL.clone());
        let value: i64 = token
            .literal
            .as_ref()
            .ok_or("empty integer")?
            .parse()
            .map_err(|_| String::from("invalid integer"))?;

        Ok(IntegerLiteral { token, value }.into())
    }

    fn parse_prefix_expression(&mut self) -> Result<Expression, String> {
        let token = std::mem::replace(&mut self.current_token, Token::ILLEGAL.clone());
        self.next_token();
        let right = self.parse_expression(Precidence::Prefix)?;

        Ok(PrefixExpression {
            token,
            right: Box::from(right),
        }
        .into())
    }

    fn current_token_is(&self, t: &TokenType) -> bool {
        self.current_token.token_type == *t
    }

    fn peek_token_is(&self, t: &TokenType) -> bool {
        self.peek_token.token_type == *t
    }

    fn expect_peek(&mut self, t: &TokenType) -> Result<(), String> {
        if self.peek_token_is(t) {
            self.next_token();
            Ok(())
        } else {
            Err(format!(
                "Expected {:?} = got {:?}",
                t, self.peek_token.token_type
            ))
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::shared::ast::{Expression, Node, Statement};
    use crate::shared::tokens::Token;

    #[test]
    fn test_let_statements() {
        let input = "
			let x = 5;
			let y = 10;
			let foobar = 838383;
			";

        let lex = Lexer::new(input);
        let mut parse = Parser::new(lex);

        let prog = parse.parse_program();
        check_errors(&parse);

        assert_eq!(
            prog.statements.len(),
            3,
            "Program not three statements long. got={}",
            prog.statements.len()
        );

        let expected = ["x", "y", "foobar"];

        for (stmnt, expected) in prog.statements.iter().zip(expected) {
            test_let_statement(stmnt, expected)
        }
    }

    #[test]
    fn test_return_statements() {
        let input = "
			return 5;
			return 10;
			return 838383;
			";

        let lex = Lexer::new(input);
        let mut parse = Parser::new(lex);

        let prog = parse.parse_program();
        check_errors(&parse);

        assert_eq!(
            prog.statements.len(),
            3,
            "Program not three statements long. got={}",
            prog.statements.len()
        );

        // let expected = ["x", "y", "foobar"];
        let mut errors = Vec::new();
        for stmnt in prog.statements.iter() {
            match stmnt {
                Statement::Return { .. } => continue,
                _ => errors.push(format!("Did not get Return statement, got={:?}", stmnt)),
            };
        }
        if !errors.is_empty() {
            panic_with_errors(&errors);
        }
    }

    #[test]
    fn test_identifier_expression() {
        let input = "foobar;";

        let lex = Lexer::new(input);
        let mut parse = Parser::new(lex);

        let prog = parse.parse_program();
        check_errors(&parse);

        assert_eq!(
            prog.statements.len(),
            1,
            "Program not one statement long. got={}",
            prog.statements.len()
        );

        let Statement::Expression { ref value, .. } = prog.statements[0] else {
            panic!("Not Expression. got {:?}", prog.statements[0]);
        };

        let Expression::Identifier(Identifier { token }) = value else {
            panic!("Not Identifier. got {:?}", value);
        };

        assert_eq!(token.token_type, TokenType::Identifier);
        assert_eq!(token.literal, Some("foobar".into()));
    }

    #[test]
    fn test_integer_literal() {
        let input = "5;";

        let lex = Lexer::new(input);
        let mut parse = Parser::new(lex);

        let prog = parse.parse_program();
        check_errors(&parse);

        assert_eq!(
            prog.statements.len(),
            1,
            "Program not one statement long. got={}",
            prog.statements.len()
        );

        let Statement::Expression { ref value, .. } = prog.statements[0] else {
            panic!("Not Expression. got {:?}", prog.statements[0]);
        };

        let Expression::IntegerLiteral(IntegerLiteral { token, value }) = value else {
            panic!("Not IntegerLiteral. got {:?}", value);
        };

        assert_eq!(token.token_type, TokenType::Integer);
        assert_eq!(token.literal, Some("5".into()));
        assert_eq!(*value, 5);
    }

    #[test]
    fn test_prefix_expression() {
        let inputs = [("!5;", TokenType::Bang, 5), ("-15;", TokenType::Minus, 15)];

        for (input, token_type, expected) in inputs {
            let lex = Lexer::new(input);
            let mut parse = Parser::new(lex);

            let prog = parse.parse_program();
            check_errors(&parse);

            assert_eq!(
                prog.statements.len(),
                1,
                "Program not one statement long. got={}",
                prog.statements.len()
            );

            let Statement::Expression { ref value, .. } = prog.statements[0] else {
                panic!("Not Expression. got {:?}", prog.statements[0]);
            };

            let Expression::PrefixExpression(PrefixExpression { token, right }) = value else {
                panic!("Not PrefixExpression. got {:?}", value);
            };
            assert_eq!(token.token_type, token_type);

            let Expression::IntegerLiteral(IntegerLiteral {
                ref token,
                ref value,
            }) = **right
            else {
                panic!("Not IntegerLiteral. got {:?}", value);
            };

            assert_eq!(token.token_type, TokenType::Integer);
            assert_eq!(token.literal, Some(expected.to_string()));
            assert_eq!(*value, expected);
        }
    }

    fn test_let_statement(stmnt: &Statement, expected: &str) {
        let Statement::Let {
            token,
            name,
            value: _,
        } = stmnt
        else {
            panic!("Not let statement. Got {:?}", stmnt);
        };

        assert_eq!(
            token.token_type,
            TokenType::Let,
            "Not let token. Got {:?}",
            token
        );
        assert_eq!(
            name.string(),
            expected,
            "Not correct name. Got {} instead of {}",
            name.string(),
            expected
        );
        assert_eq!(
            name.token(),
            Some(&Token::new_ident(expected.into())),
            "Not correct token. Got {:?}",
            name.token()
        );
    }

    fn check_errors(p: &Parser) {
        if p.errors.is_empty() {
            return;
        };

        panic_with_errors(&p.errors);
    }

    fn panic_with_errors(e: &[String]) {
        let mut errors = String::from("Parsing failed with errors: \n");
        for error in e {
            errors.push_str(&format!(". {}", error))
        }
        panic!("{}", errors);
    }
}
