use std::fmt::Display;

#[derive(Debug, PartialEq, Eq, Clone, Hash)]
pub enum TokenType {
    Illegal,
    EOF,

    // Identifiers & literals
    Identifier,
    Integer,

    // Operators
    Assign,
    Equal,
    Plus,
    Minus,
    Asterisk,
    ForwardSlash,
    Dot,
    Bang,
    NotEqual,
    LessThan,
    LessThanOrEqual,
    GreaterThan,
    GreaterThanOrEqual,

    // Delimiters
    Comma,
    Semicolon,

    LParen,
    RParen,
    LBrace,
    RBrace,

    // Keywords
    Function,
    Let,
    True,
    False,
    If,
    Else,
    Return,
}

use TokenType as T;

impl Display for TokenType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let x = match self {
            T::Assign => "=",
            T::Equal => "==",
            T::Plus => "+",
            T::Minus => "-",
            T::Asterisk => "*",
            T::ForwardSlash => "/",
            T::Dot => ".",
            T::Bang => "!",
            T::NotEqual => "!=",
            T::LessThan => "<",
            T::LessThanOrEqual => "<=",
            T::GreaterThan => ">",
            T::GreaterThanOrEqual => ">=",

            T::Comma => ",",
            T::Semicolon => ";",

            T::LParen => "(",
            T::RParen => ")",
            T::LBrace => "{",
            T::RBrace => "}",

            T::Function => "fn",
            T::Let => "let",
            T::True => "true",
            T::False => "false",
            T::If => "if",
            T::Else => "else",
            T::Return => "return",

            _ => "",
        };
        write!(f, "{}", x)
    }
}

#[derive(Debug, PartialEq, Eq, Clone)]
pub struct Token {
    pub token_type: TokenType,
    pub literal: Option<String>,
}

impl Token {
    pub const EOF: Token = Token {
        token_type: TokenType::EOF,
        literal: None,
    };
    pub const ILLEGAL: Token = Token {
        token_type: TokenType::Illegal,
        literal: None,
    };

    pub fn new(token: TokenType) -> Token {
        Token {
            token_type: token,
            literal: None,
        }
    }

    pub fn new_ident(value: String) -> Token {
        Token {
            token_type: TokenType::Identifier,
            literal: Some(value),
        }
    }

    pub fn new_integer(value: String) -> Token {
        Token {
            token_type: TokenType::Integer,
            literal: Some(value),
        }
    }
}

impl From<TokenType> for Token {
    fn from(value: TokenType) -> Self {
        Token::new(value)
    }
}

impl Display for Token {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let x = match self.token_type {
            T::Identifier => self.literal.clone().expect("empty identifier"),
            T::Integer => self.literal.clone().expect("empty identifier"),
            _ => self.token_type.to_string(),
        };
        write!(f, "{}", x)
    }
}
