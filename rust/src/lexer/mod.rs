use crate::{Token, TokenType as T};

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
        }
        _ => not,
    }
}

impl Lexer {
    pub fn new(input: &str) -> Lexer {
        let mut lex = Lexer {
            chars: input.chars().collect(),
            position: 0,
            read_position: 0,
            ch: None,
        };
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

    pub fn next_token(&mut self) -> Token {
        let Some(c) = self.ch else {
            return Token::EOF.clone();
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
        while matcher(self.ch.unwrap()) {
            ident.push(self.ch.unwrap());
            self.read_next_char();
        }

        ident.iter().collect()
    }

    fn match_identifier(&mut self) -> Token {
        let ident: String = self.match_characters(is_valid_identifier_char);

        match ident.as_str() {
            "let" => T::Let.into(),
            "fn" => T::Function.into(),
            "true" => T::True.into(),
            "false" => T::False.into(),
            "if" => T::If.into(),
            "else" => T::Else.into(),
            "return" => T::Return.into(),
            _ => Token::new_ident(ident),
        }
    }

    fn match_number(&mut self) -> Token {
        let ident: String = self.match_characters(is_valid_number_char);

        Token::new_integer(ident)
    }

    fn match_symbol(&mut self) -> Token {
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
        t.into()
    }

    fn skip_whitespace(&mut self) {
        while let Some(c) = self.ch {
            if c.is_whitespace() {
                self.read_next_char()
            } else {
                break;
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn symbols() {
        let input = "=+(){},;";

        let mut lex = Lexer::new(input);

        let expected = [
            T::Assign,
            T::Plus,
            T::LParen,
            T::RParen,
            T::LBrace,
            T::RBrace,
            T::Comma,
            T::Semicolon,
            T::EOF,
        ];

        for token in expected {
            assert_eq!(token, lex.next_token().token_type);
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
            Token::new(T::Let),
            Token::new_ident("five".into()),
            Token::new(T::Assign),
            Token::new_integer("5".into()),
            Token::new(T::Semicolon),
            Token::new(T::Let),
            Token::new_ident("ten".into()),
            Token::new(T::Assign),
            Token::new_integer("10".into()),
            Token::new(T::Semicolon),
            Token::new(T::Let),
            Token::new_ident("add".into()),
            Token::new(T::Assign),
            Token::new(T::Function),
            Token::new(T::LParen),
            Token::new_ident("x".into()),
            Token::new(T::Comma),
            Token::new_ident("y".into()),
            Token::new(T::RParen),
            Token::new(T::LBrace),
            Token::new_ident("x".into()),
            Token::new(T::Plus),
            Token::new_ident("y".into()),
            Token::new(T::Semicolon),
            Token::new(T::RBrace),
            Token::new(T::Semicolon),
            Token::new(T::Let),
            Token::new_ident("result".into()),
            Token::new(T::Assign),
            Token::new_ident("add".into()),
            Token::new(T::LParen),
            Token::new_ident("five".into()),
            Token::new(T::Comma),
            Token::new_ident("ten".into()),
            Token::new(T::RParen),
            Token::new(T::Semicolon),
            Token::new(T::EOF),
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
            Token::new(T::Let),
            Token::new_ident("five".into()),
            Token::new(T::Assign),
            Token::new_integer("5".into()),
            Token::new(T::Semicolon),
            Token::new(T::Let),
            Token::new_ident("ten".into()),
            Token::new(T::Assign),
            Token::new_integer("10".into()),
            Token::new(T::Semicolon),
            Token::new(T::Let),
            Token::new_ident("add".into()),
            Token::new(T::Assign),
            Token::new(T::Function),
            Token::new(T::LParen),
            Token::new_ident("x".into()),
            Token::new(T::Comma),
            Token::new_ident("y".into()),
            Token::new(T::RParen),
            Token::new(T::LBrace),
            Token::new_ident("x".into()),
            Token::new(T::Plus),
            Token::new_ident("y".into()),
            Token::new(T::Semicolon),
            Token::new(T::RBrace),
            Token::new(T::Semicolon),
            Token::new(T::Let),
            Token::new_ident("result".into()),
            Token::new(T::Assign),
            Token::new_ident("add".into()),
            Token::new(T::LParen),
            Token::new_ident("five".into()),
            Token::new(T::Comma),
            Token::new_ident("ten".into()),
            Token::new(T::RParen),
            Token::new(T::Semicolon),
            Token::new(T::Bang),
            Token::new(T::Minus),
            Token::new(T::ForwardSlash),
            Token::new(T::Asterisk),
            Token::new_integer("5".into()),
            Token::new(T::Semicolon),
            Token::new_integer("5".into()),
            Token::new(T::LessThan),
            Token::new_integer("10".into()),
            Token::new(T::GreaterThan),
            Token::new_integer("5".into()),
            Token::new(T::Semicolon),
            Token::new(T::If),
            Token::new(T::LParen),
            Token::new_integer("5".into()),
            Token::new(T::LessThan),
            Token::new_integer("10".into()),
            Token::new(T::RParen),
            Token::new(T::LBrace),
            Token::new(T::Return),
            Token::new(T::True),
            Token::new(T::Semicolon),
            Token::new(T::RBrace),
            Token::new(T::Else),
            Token::new(T::LBrace),
            Token::new(T::Return),
            Token::new(T::False),
            Token::new(T::Semicolon),
            Token::new(T::RBrace),
            Token::new_integer("10".into()),
            Token::new(T::Equal),
            Token::new_integer("10".into()),
            Token::new(T::Semicolon),
            Token::new_integer("10".into()),
            Token::new(T::NotEqual),
            Token::new_integer("9".into()),
            Token::new(T::Semicolon),
            Token::new(T::LessThanOrEqual),
            Token::new(T::GreaterThanOrEqual),
            Token::new(T::EOF),
        ];

        for token in expected {
            assert_eq!(token, lex.next_token());
        }
    }
}
