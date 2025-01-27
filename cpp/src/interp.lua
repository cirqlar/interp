project "interp"
	kind "ConsoleApp"
	targetdir "%{outputdir}/"
	objdir "%{interdir}/"
	staticruntime "off"

	files { "main.cpp" }

	includedirs { "./" }

	links { "interp-repl" }

include "repl/interp-repl.lua"
include "parser/interp-parser.lua"
include "lexer/interp-lexer.lua"
