package parser

import (
	"fmt"
	"interp/ast"
	"interp/lexer"
	"testing"
)

func TestLetStatements(t *testing.T) {
	input := `
let x = 5;
let y = 10;
let foobar = 838383;
`
	l := lexer.New(input)
	p := New(l)

	program := p.ParseProgram()
	checkParserErrors(t, p)
	if program == nil {
		t.Fatalf("ParseProgram() returned nil")
	}

	if len(program.Statements) != 3 {
		t.Fatalf("program.Statements does not contain 3 statements. got=%d", len(program.Statements))
	}

	tests := []struct {
		expectedIdentifier string
	}{
		{"x"},
		{"y"},
		{"foobar"},
	}

	for i, tt := range tests {
		stmnt := program.Statements[i]
		if !testLetStatement(t, stmnt, tt.expectedIdentifier) {
			return
		}
	}
}

func TestReturnStatements(t *testing.T) {
	input := `
return 5;
return 10;
return 838383;
	`

	l := lexer.New(input)
	p := New(l)

	prog := p.ParseProgram()
	checkParserErrors(t, p)
	if prog == nil {
		t.Fatal("ParseProgram return nil")
	}

	if len(prog.Statements) != 3 {
		t.Fatalf("Program has %d statements instead of 3", len(prog.Statements))
	}

	for _, stmnt := range prog.Statements {
		r, ok := stmnt.(*ast.ReturnStatement)
		if !ok {
			t.Errorf("Statement isn't *ast.ReturnStatement, got=%T", stmnt)
			continue
		}
		if r.TokenLiteral() != "return" {
			t.Errorf("r.TokenLiteral not 'return', got %q",
				r.TokenLiteral())
		}
	}
}

func TestIdentifierExpression(t *testing.T) {
	input := `foobar;`

	l := lexer.New(input)
	p := New(l)

	prog := p.ParseProgram()
	checkParserErrors(t, p)
	if prog == nil {
		t.Fatal("ParseProgram returned nil")
	}

	if len(prog.Statements) != 1 {
		t.Errorf("Program has %d statments instead of 1", len(prog.Statements))
	}

	stmnt, ok := prog.Statements[0].(*ast.ExpressionStatement)
	if !ok {
		t.Fatalf("Statment is not an *ast.ExpressionStatement, got %T", prog.Statements[0])
	}

	ident, ok := stmnt.Expression.(*ast.Identifier)
	if !ok {
		t.Fatalf("Expression is not an *ast.Identifier, got %T", stmnt.Expression)
	}

	if ident.Value != "foobar" {
		t.Errorf("Value is not %s. got %s", "foobar", ident.Value)
	}
	if ident.TokenLiteral() != "foobar" {
		t.Errorf("TokenLiteral not %s. got %s", "foobar", ident.TokenLiteral())
	}
}

func TestIntegerLiteral(t *testing.T) {
	input := `5;`

	l := lexer.New(input)
	p := New(l)

	prog := p.ParseProgram()
	checkParserErrors(t, p)
	if prog == nil {
		t.Fatal("ParseProgram returned nil")
	}

	if len(prog.Statements) != 1 {
		t.Errorf("Program has %d statments instead of 1", len(prog.Statements))
	}

	stmnt, ok := prog.Statements[0].(*ast.ExpressionStatement)
	if !ok {
		t.Fatalf("Statment is not an *ast.ExpressionStatement, got %T", prog.Statements[0])
	}

	lit, ok := stmnt.Expression.(*ast.IntegerLiteral)
	if !ok {
		t.Fatalf("Expression is not an *ast.IntegerLiteral, got %T", stmnt.Expression)
	}

	if lit.Value != 5 {
		t.Errorf("Value is not %d. got %d", 5, lit.Value)
	}
	if lit.TokenLiteral() != "5" {
		t.Errorf("TokenLiteral not %s. got %s", "5", lit.TokenLiteral())
	}
}

func TestPrefixExpression(t *testing.T) {
	prefixTests := []struct {
		input        string
		operator     string
		integerValue int64
	}{
		{"!5", "!", 5},
		{"-15", "-", 15},
	}

	for _, tt := range prefixTests {
		l := lexer.New(tt.input)
		p := New(l)

		prog := p.ParseProgram()
		checkParserErrors(t, p)
		if prog == nil {
			t.Error("ParseProgram returned nil")
			continue
		}

		if len(prog.Statements) != 1 {
			t.Errorf("there is not 1 Statment, got %d", len(prog.Statements))
			continue
		}

		stmnt, ok := prog.Statements[0].(*ast.ExpressionStatement)
		if !ok {
			t.Errorf("stmnt was not *ast.ExpressionStatment, got %T", prog.Statements[0])
			continue
		}

		pre, ok := stmnt.Expression.(*ast.PrefixExpression)
		if !ok {
			t.Errorf("expression was not *ast.PrefixExpression, got %T", stmnt.Expression)
			continue
		}

		if pre.Operator != tt.operator {
			t.Errorf("operator was %s instead of %s", pre.Operator, tt.operator)
		}

		testIntegerLiteral(t, pre.Right, tt.integerValue)
	}
}

func testLetStatement(t *testing.T, s ast.Statement, name string) bool {
	if s.TokenLiteral() != "let" {
		t.Errorf("s.TokenLiteral not 'let'. got=%q", s.TokenLiteral())
		return false
	}
	letStmt, ok := s.(*ast.LetStatement)
	if !ok {
		t.Errorf("s not *ast.LetStatement. got=%T", s)
		return false
	}
	if letStmt.Name.Value != name {
		t.Errorf("letStmt.Name.Value not '%s'. got=%s", name, letStmt.Name.Value)
		return false
	}
	if letStmt.Name.TokenLiteral() != name {
		t.Errorf("s.Name not '%s'. got=%s", name, letStmt.Name)
		return false
	}
	return true
}

func checkParserErrors(t *testing.T, p *Parser) {
	errors := p.Errors()
	if len(errors) == 0 {
		return
	}
	t.Errorf("parser has %d errors", len(errors))
	for _, msg := range errors {
		t.Errorf("parser error: %q", msg)
	}
	t.FailNow()
}

func testIntegerLiteral(t *testing.T, il ast.Expression, value int64) bool {
	integ, ok := il.(*ast.IntegerLiteral)
	if !ok {
		t.Errorf("il not *ast.IntegerLiteral. got=%T", il)
		return false
	}
	if integ.Value != value {
		t.Errorf("integ.Value not %d. got=%d", value, integ.Value)
		return false
	}
	if integ.TokenLiteral() != fmt.Sprintf("%d", value) {
		t.Errorf("integ.TokenLiteral not %d. got=%s", value,
			integ.TokenLiteral())
		return false
	}
	return true
}
