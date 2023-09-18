#[derive(Debug, PartialEq, Clone)]
pub enum Token {
	Illegal,
	EOF,

	// Identifiers & literals
	Identifier(String),
	Integer(String),

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