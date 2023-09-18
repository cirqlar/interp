use std::fmt::Debug;

use crate::Token;

pub trait Node: Debug {
    fn token(&self) -> Option<&Token>;
}

pub trait Expression: Node {}

#[derive(Debug, Default)]
pub struct Program {
    pub(crate) statements: Vec<Statement>,
}

impl Program {
	pub fn new() -> Program {
		Program::default()
	}
}

impl Node for Program {
    fn token(&self) -> Option<&Token> {
        if !self.statements.is_empty() {
            self.statements[0].token()
        } else {
            None
        }
    }
}

#[derive(Debug)]
pub enum Statement {
    Let {
        token: Token,
        name: Identifier,
        value: Option<Box<dyn Expression>>,
    },
}
use Statement as S;

impl Node for Statement {
    fn token(&self) -> Option<&Token> {
        match self {
            S::Let {
                token,
                name: _,
                value: _,
            } => Some(token),
            _ => panic!("at the disco"),
        }
    }
}

#[derive(Debug)]
pub struct Identifier {
    pub(crate) token: Token,
    pub value: String,
}

impl Node for Identifier {
    fn token(&self) -> Option<&Token> {
        Some(&self.token)
    }
}
impl Expression for Identifier {}


