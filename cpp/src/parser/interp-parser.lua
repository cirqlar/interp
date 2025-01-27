project "interp-parser"
	kind "StaticLib"
	targetdir "%{outputdir}/parser"
	objdir "%{interdir}/parser"
	staticruntime "off"

	files { "**.cpp" }

	includedirs { "./", "%{maindir}/src/" }

	links { "interp-lexer" }
