use crate::{lexer::Lexer, Token, shared::ast::{Program, Statement, Identifier}};

pub struct Parser {
	l: Lexer,
	current_token: Token,
	peek_token: Token,
	pub errors: Vec<String>,
}

impl Parser {
	pub fn new(l: Lexer) -> Parser {
		let mut p = Parser { l, current_token: Token::Illegal, peek_token: Token::Illegal, errors: Vec::new() };

		p.next_token();
		p.next_token();

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
		match self.current_token {
			Token::Let => self.parse_let_statement(),
			_ => Err("Not handled".into()),
		}
	}

	fn parse_let_statement(&mut self) -> Result<Statement, String> {
		let current_token = std::mem::replace(&mut self.current_token, Token::Illegal);

		let peek_token = std::mem::replace(&mut self.peek_token, Token::Illegal);
		let name = match peek_token {
			Token::Identifier(ref name) => {
				self.next_token();
				Ok::<_,String>(name.clone())
			},
			_ => return Err(format!("Expected identifier got {:?}", peek_token)),
		}?;

		let name = Identifier { token: peek_token, value: name };

		if !self.expect_peek(Token::Assign) {
			return Err(format!("Expected = got {:?}", self.peek_token));
		}

		// Skip till next semicolon
		while !self.current_token_is(Token::Semicolon) {
			self.next_token();
		}

		Ok(Statement::Let { token: current_token, name, value: None })
	}

	fn current_token_is(&self, t: Token) -> bool {
		self.current_token == t
	}

	fn peek_token_is(&self, t: Token) -> bool {
		self.peek_token == t
	}

	fn expect_peek(&mut self, t: Token) -> bool {
		if self.peek_token_is(t) {
			self.next_token();
			true
		} else {
			false
		}
	}
}

#[cfg(test)]
mod tests {
    use super::*;
	use crate::shared::ast::{Statement, Node};
	use crate::shared::tokens::Token;

    #[test]
    fn test_let_statments() {
        let input = "
			let x = 5;
			let y = 10;
			let foobar = 838383;
			";
		
		let lex = Lexer::new(input);
		let mut parse = Parser::new(lex);

		let prog = parse.parse_program();
		check_errors(&parse);
		
		assert_eq!(prog.statements.len(), 3, "Program not three statments long. got={}", prog.statements.len());

		let expected = ["x", "y", "foobar"];

		for (stmnt, expected) in prog.statements.iter().zip(expected) {
			test_let_statement(stmnt, expected)
		}




    }

	fn test_let_statement(stmnt: &Statement, expected: &str) {
		let Statement::Let { token, name, value: _  } = stmnt else {
			panic!("Not let statement. Got {:?}", stmnt);
		};

		assert_eq!(*token, Token::Let, "Not let token. Got {:?}", token);
		assert_eq!(name.value, expected, "Not correct name. Got {} instead of {}", name.value, expected);
		assert_eq!(name.token(), Some(&Token::Identifier(expected.into())), "Not correct token. Got {:?}", name.token());
	}

	fn check_errors(p: &Parser) {
		if p.errors.is_empty() {
			return
		};
	
		let mut errors = String::from("Parsing failed with errors: \n");
		for error in p.errors.iter() {
			errors.push_str(&format!(". {}", error))
		}
		panic!("{}", errors);
	}
}