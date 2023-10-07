use std::fmt::Debug;

use crate::Token;

pub trait Node: Debug {
    fn token(&self) -> Option<&Token>;
    fn string(&self) -> String;
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

    fn string(&self) -> String {
        let mut out = String::new();
        for stmnt in self.statements.iter() {
            out.push_str(&stmnt.string());
        }

        out
    }
}

#[derive(Debug)]
pub enum Statement {
    Let {
        token: Token,
        name: Identifier,
        value: Option<Box<dyn Expression>>,
    },
    Return {
        token: Token,
        value: Option<Box<dyn Expression>>,
    },
    Expression {
        token: Token,
        value: Box<dyn Expression>,
    },
}
use Statement as S;

impl Statement {
    fn let_string(&self) -> String {
        let S::Let { name, value, .. } = self else {
            panic!("Called let_string with non Let statement");
        };

        let mut out = String::from("let ");
        out.push_str(&format!("{} = ", name.value));
        if let Some(expr) = value {
            out.push_str(&expr.string());
        }
        out.push(';');

        out
    }

    fn return_string(&self) -> String {
        let S::Return { value, .. } = self else {
            panic!("Called return_string with non Return Statement");
        };

        let mut out = String::from("return ");
        if let Some(expr) = value {
            out.push_str(&expr.string());
        }
        out.push(';');

        out
    }

    fn expression_string(&self) -> String {
        let S::Expression { value, .. } = self else {
            panic!("Called expression_string with non Expression Statement");
        };

        value.string()
    }
}

impl Node for Statement {
    fn token(&self) -> Option<&Token> {
        match self {
            S::Let { token, .. } => Some(token),
            S::Return { token, .. } => Some(token),
            S::Expression { token, .. } => Some(token),
            // _ => panic!("at the disco"),
        }
    }

    fn string(&self) -> String {
        match self {
            S::Let { .. } => self.let_string(),
            S::Return { .. } => self.return_string(),
            S::Expression { .. } => self.expression_string(),
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

    fn string(&self) -> String {
        self.value.clone()
    }
}
impl Expression for Identifier {}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_string() {
        let prog = Program {
            statements: vec![Statement::Let {
                token: Token::Let,
                name: Identifier {
                    token: Token::Identifier("myVar".into()),
                    value: "myVar".into(),
                },
                value: Some(Box::from(Identifier {
                    token: Token::Identifier("anotherVar".into()),
                    value: "anotherVar".into(),
                })),
            }],
        };

        assert_eq!(prog.string(), String::from("let myVar = anotherVar;"));
    }
}
