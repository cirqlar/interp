use crate::Token as T;

pub struct Lexer {
	chars: Vec<char>,
	position: usize,
	read_position: usize,
	ch: Option<char>,
}

fn is_valid_identifier_char(c: char) -> bool {
	c.is_alphabetic() || c == '_' || c.is_ascii_digit()
}

fn is_valid_number_char(c: char) -> bool {
	c.is_ascii_hexdigit() || c == '_'
}

fn or_equal(lex: &mut Lexer, eq: T, not: T) -> T {
	match lex.peek_next_char() {
		Some(x) if x == '=' => {
			lex.read_next_char();
			eq
		},
		_ => not,
	}
}

impl Lexer {
	pub fn new(input: &str) -> Lexer {
		let mut lex = Lexer { chars: input.chars().collect(), position: 0, read_position: 0, ch: None };
		lex.read_next_char();
		lex
	}

	fn read_next_char(&mut self) {
		self.ch = self.chars.get(self.read_position).copied();
		self.position = self.read_position;
		self.read_position += 1;
	}
	
	fn peek_next_char(&self) -> Option<char> {
		self.chars.get(self.read_position).copied()
	}

	pub fn next_token(&mut self) -> T {
		let Some(c) = self.ch else {
			return T::EOF
		};

		match c {
			x if char::is_whitespace(x) => {
				self.skip_whitespace();
				self.next_token()
			}
			x if !is_valid_identifier_char(x) => self.match_symbol(),
			x if char::is_alphabetic(x) => self.match_identifier(),
			x if char::is_digit(x, 10) => self.match_number(),
			_ => panic!("Unknown symbol"),
		}
	}

	fn match_characters(&mut self, matcher: fn(char) -> bool) -> String {
		let mut ident = vec![self.ch.unwrap()];
		self.read_next_char();
		while matcher( self.ch.unwrap() ) {
			ident.push(self.ch.unwrap());
			self.read_next_char();
		}
		
		ident.iter().collect()
	}

	fn match_identifier(&mut self) -> T {
		let ident: String = self.match_characters(is_valid_identifier_char);

		match ident.as_str() {
			"let" => T::Let,
			"fn" => T::Function,
			"true" => T::True,
			"false" => T::False,
			"if" => T::If,
			"else" => T::Else,
			"return" => T::Return,
			_ => T::Identifier(ident),
		}
	}

	fn match_number(&mut self) -> T {
		let ident: String = self.match_characters(is_valid_number_char);

		T::Integer(ident)
	}

	fn match_symbol(&mut self) -> T {	
		let t = match self.ch.unwrap() {
			'=' => or_equal(self, T::Equal, T::Assign),
			'+' => T::Plus,
			'-' => T::Minus,
			'*' => T::Asterisk,
			'/' => T::ForwardSlash,
			'(' => T::LParen,
			')' => T::RParen,
			'{' => T::LBrace,
			'}' => T::RBrace,
			',' => T::Comma,
			';' => T::Semicolon,
			'.' => T::Dot,
			'!' => or_equal(self, T::NotEqual, T::Bang),
			'<' => or_equal(self, T::LessThanOrEqual, T::LessThan),
			'>' => or_equal(self, T::GreaterThanOrEqual, T::GreaterThan),
			_ => T::Illegal,
		};

		self.read_next_char();
		t
	}

	fn skip_whitespace(&mut self) {
		while let Some(c) = self.ch {
			if c.is_whitespace() {
				self.read_next_char()
			} else {
				break;
			}
		};
	}
}

#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn symbols() {
		let input = "=+(){},;";

		let mut lex = Lexer::new(input);

		let expected = [T::Assign, T::Plus, T::LParen, T::RParen, T::LBrace, T::RBrace, T::Comma, T::Semicolon, T::EOF];

		for token in expected {
			
			assert_eq!(token, lex.next_token());
		}
	}

	#[test]
	fn basic_example() {
		let input = "let five = 5;
			let ten = 10;
			let add = fn(x, y) {
			x + y;
			};
			let result = add(five, ten);
		";

		let mut lex = Lexer::new(input);

		let expected = [
			T::Let,
			T::Identifier("five".into()),
			T::Assign,
			T::Integer("5".into()),
			T::Semicolon,
			T::Let,
			T::Identifier("ten".into()),
			T::Assign,
			T::Integer("10".into()),
			T::Semicolon,
			T::Let,
			T::Identifier("add".into()),
			T::Assign,
			T::Function,
			T::LParen,
			T::Identifier("x".into()),
			T::Comma,
			T::Identifier("y".into()),
			T::RParen,
			T::LBrace,
			T::Identifier("x".into()),
			T::Plus,
			T::Identifier("y".into()),
			T::Semicolon,
			T::RBrace,
			T::Semicolon,
			T::Let,
			T::Identifier("result".into()),
			T::Assign,
			T::Identifier("add".into()),
			T::LParen,
			T::Identifier("five".into()),
			T::Comma,
			T::Identifier("ten".into()),
			T::RParen,
			T::Semicolon,
			T::EOF,
		];

		for token in expected {
			assert_eq!(token, lex.next_token());
		}
	}

	#[test]
	fn basic_example_2() {
		let input = "let five = 5;
			let ten = 10;
			let add = fn(x, y) {
			x + y;
			};
			let result = add(five, ten);
			!-/*5;
			5 < 10 > 5;
			if (5 < 10) {
			return true;
			} else {
			return false;
			}
			10 == 10;
			10 != 9;
			<=
			>=
		";

		let mut lex = Lexer::new(input);

		let expected = [
			T::Let,
			T::Identifier("five".into()),
			T::Assign,
			T::Integer("5".into()),
			T::Semicolon,
			T::Let,
			T::Identifier("ten".into()),
			T::Assign,
			T::Integer("10".into()),
			T::Semicolon,
			T::Let,
			T::Identifier("add".into()),
			T::Assign,
			T::Function,
			T::LParen,
			T::Identifier("x".into()),
			T::Comma,
			T::Identifier("y".into()),
			T::RParen,
			T::LBrace,
			T::Identifier("x".into()),
			T::Plus,
			T::Identifier("y".into()),
			T::Semicolon,
			T::RBrace,
			T::Semicolon,
			T::Let,
			T::Identifier("result".into()),
			T::Assign,
			T::Identifier("add".into()),
			T::LParen,
			T::Identifier("five".into()),
			T::Comma,
			T::Identifier("ten".into()),
			T::RParen,
			T::Semicolon,
			T::Bang,
			T::Minus,
			T::ForwardSlash,
			T::Asterisk,
			T::Integer("5".into()),
			T::Semicolon,
			T::Integer("5".into()),
			T::LessThan,
			T::Integer("10".into()),
			T::GreaterThan,
			T::Integer("5".into()),
			T::Semicolon,
			T::If,
			T::LParen,
			T::Integer("5".into()),
			T::LessThan,
			T::Integer("10".into()),
			T::RParen,
			T::LBrace,
			T::Return,
			T::True,
			T::Semicolon,
			T::RBrace,
			T::Else,
			T::LBrace,
			T::Return,
			T::False,
			T::Semicolon,
			T::RBrace,
			T::Integer("10".into()),
			T::Equal,
			T::Integer("10".into()),
			T::Semicolon,
			T::Integer("10".into()),
			T::NotEqual,
			T::Integer("9".into()),
			T::Semicolon,
			T::LessThanOrEqual,
			T::GreaterThanOrEqual,
			T::EOF,
		];

		for token in expected {
			assert_eq!(token, lex.next_token());
		}
	}
}