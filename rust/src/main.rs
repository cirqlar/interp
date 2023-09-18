use interp::repl::Repl;

fn main() {
    Repl::start()
    // let input = "let five = 5;
    //     let ten = 10;
    //     let add = fn(x, y) {
    //     x + y;
    //     };
    //     let result = add(five, ten);
    //     !-/*5;
    //     5 < 10 > 5;
    //     if (5 < 10) {
    //     return true;
    //     } else {
    //     return false;
    //     }
    //     10 == 10;
    //     10 != 9;
    // ";

    // let mut lex = Lexer::new(input.into());

    // loop {
    //     let t = lex.next_token();
    //     println!("{:?},", t);
    //     if t == T::EOF || t == T::Illegal {
    //         break;
    //     }
    // }
}
