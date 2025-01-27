project "interp-repl"
	kind "StaticLib"
	targetdir "%{outputdir}/repl"
	objdir "%{interdir}/repl"
	staticruntime "off"

	files { "**.cpp" }

	includedirs { "./", "%{maindir}/src/" }

	links { "interp-lexer", "interp-parser" }
