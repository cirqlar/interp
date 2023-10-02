package parser

import (
	"interp/ast"
	"interp/lexer"
	"testing"
)

func TestLetStatments(t *testing.T) {
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
		if !testLetStatment(t, stmnt, tt.expectedIdentifier) {
			return
		}
	}
}

func TestReturnStatments(t *testing.T) {
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
			t.Errorf("Statment isn't *ast.ReturnStatement, got=%T", stmnt)
			continue
		}
		if r.TokenLiteral() != "return" {
			t.Errorf("r.TokenLiteral not 'return', got %q",
				r.TokenLiteral())
		}
	}
}

func testLetStatment(t *testing.T, s ast.Statement, name string) bool {
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
