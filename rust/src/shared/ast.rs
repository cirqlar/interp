use crate::Token;



struct Program {
	statements: Vec<Statement>,
}

trait Node {
	fn token(&self) -> &Token;
}

struct Identifier {
	token: Token,
	value: String,
}

impl Node for Identifier {
	fn token(&self) -> &Token {
		&self.token
	}
}

struct Expression {}

enum Statement {
	Let { token: Token, name: Identifier, value: Expression }
}

use Statement as S;

impl Node for Statement {

	fn token(&self) -> &Token {
		match self {
			S::Let { token, name: _, value: _ } => token,
			_ => panic!("at the disco"),
 		}
	}
}