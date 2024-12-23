cmake_minimum_required(VERSION 3.28)

add_library(interp_parser STATIC 
	"parser.cpp"
	"ast/node.cpp"
	"ast/block.cpp"
	"ast/bool.cpp"
	"ast/expr.cpp"
	"ast/ident.cpp"
	"ast/if.cpp"
	"ast/infix.cpp"
	"ast/int.cpp"
	"ast/let.cpp"
	"ast/prefix.cpp"
	"ast/return.cpp"
	"ast/program.cpp"
	"ast/fn_literal.cpp"
	"ast/call.cpp"
	"ast/ast_string.cpp"

	"object/base_obj.cpp"
	"object/int_obj.cpp"
	"object/error_obj.cpp"
	"object/bool_obj.cpp"
	"object/null_obj.cpp"
	"object/return_obj.cpp"
	"object/environment.cpp"
	"object/func_obj.cpp"
	"object/string_obj.cpp"
	"object/builtin_fn.cpp"
	
	"eval.cpp"
	"builtins/builtins.cpp"
)
target_include_directories(interp_parser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(interp_parser PUBLIC interp_lexer)

set_target_properties(interp_parser
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON	
)