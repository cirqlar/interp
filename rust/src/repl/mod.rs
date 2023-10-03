use std::io;
use std::io::Write;

use crate::{lexer::Lexer, Token};

pub struct Repl();

impl Repl {
    pub fn start() {
        println!("Starting Repl...");
        let stdin = io::stdin();
        let mut input = String::new();

        loop {
            input.clear();
            print!(">> ");
            io::stdout().flush().unwrap();

            let Ok(_) = stdin.read_line(&mut input) else {
                panic!("at the disco");
            };
            let mut lex = Lexer::new(&input);

            let mut tok = Token::Dot;

            while tok != Token::EOF && tok != Token::Illegal {
                tok = lex.next_token();
                println!("T::{:?}", tok);
            }
        }
    }
}
