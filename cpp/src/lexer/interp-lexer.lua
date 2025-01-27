project "interp-lexer"
	kind "StaticLib"
	targetdir "%{outputdir}/lexer"
	objdir "%{interdir}/lexer"
	staticruntime "off"

	files { "**.cpp" }

	includedirs { "./" }
