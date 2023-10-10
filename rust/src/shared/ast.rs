use std::fmt::Debug;

use crate::Token;

pub trait Node: Debug {
    fn token(&self) -> Option<&Token>;
    fn string(&self) -> String;
}

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
        value: Option<Expression>,
    },
    Return {
        token: Token,
        value: Option<Expression>,
    },
    Expression {
        token: Token,
        value: Expression,
    },
}
use Statement as S;

impl Statement {
    fn let_string(&self) -> String {
        let S::Let { name, value, .. } = self else {
            panic!("Called let_string with non Let statement");
        };

        let mut out = String::from("let ");
        out.push_str(&format!("{} = ", name.string()));
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
}

impl Node for Identifier {
    fn token(&self) -> Option<&Token> {
        Some(&self.token)
    }

    fn string(&self) -> String {
        self.token.to_string()
    }
}

#[derive(Debug)]
pub struct IntegerLiteral {
    pub(crate) token: Token,
    pub value: i64,
}

impl Node for IntegerLiteral {
    fn token(&self) -> Option<&Token> {
        Some(&self.token)
    }

    fn string(&self) -> String {
        self.token.literal.as_ref().map_or("".into(), |s| s.clone())
    }
}

#[derive(Debug)]
pub struct PrefixExpression {
    pub(crate) token: Token,
    pub right: Box<Expression>,
}

impl Node for PrefixExpression {
    fn token(&self) -> Option<&Token> {
        Some(&self.token)
    }

    fn string(&self) -> String {
        let mut out = String::from("(");
        out.push_str(&self.token.to_string());
        out.push_str(&self.right.string());
        out.push(')');

        out
    }
}

#[derive(Debug)]
pub enum Expression {
    Identifier(Identifier),
    IntegerLiteral(IntegerLiteral),
    PrefixExpression(PrefixExpression),
}

use Expression as E;

impl Node for Expression {
    fn token(&self) -> Option<&Token> {
        match self {
            E::Identifier(x) => x.token(),
            E::IntegerLiteral(x) => x.token(),
            E::PrefixExpression(x) => x.token(),
        }
    }

    fn string(&self) -> String {
        match self {
            E::Identifier(x) => x.string(),
            E::IntegerLiteral(x) => x.string(),
            E::PrefixExpression(x) => x.string(),
        }
    }
}

impl From<Identifier> for Expression {
    fn from(value: Identifier) -> Self {
        E::Identifier(value)
    }
}

impl From<IntegerLiteral> for Expression {
    fn from(value: IntegerLiteral) -> Self {
        E::IntegerLiteral(value)
    }
}

impl From<PrefixExpression> for Expression {
    fn from(value: PrefixExpression) -> Self {
        E::PrefixExpression(value)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::TokenType;

    #[test]
    fn test_string() {
        let prog = Program {
            statements: vec![Statement::Let {
                token: Token::new(TokenType::Let),
                name: Identifier {
                    token: Token::new_ident("myVar".into()),
                },
                value: Some(
                    Identifier {
                        token: Token::new_ident("anotherVar".into()),
                    }
                    .into(),
                ),
            }],
        };

        assert_eq!(prog.string(), String::from("let myVar = anotherVar;"));
    }
}
