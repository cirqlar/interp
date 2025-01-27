workspace "interp"
	configurations { "Test", "Debug", "Release" }
	language "C++"
	cppdialect "C++20"
	startproject "interp"

maindir = _MAIN_SCRIPT_DIR
outputdir = "%{_MAIN_SCRIPT_DIR}/build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
interdir = "%{outputdir}/intermediates"

filter "configurations:Debug"
	defines { "DEBUG" }
	symbols "On"
filter {}

filter "configurations:Debug"
	defines { "DEBUG" }
	symbols "On"
filter {}

filter "configurations:Release"
	defines { "NDEBUG" }
	optimize "On"
filter {}

filter "system:windows"
	platforms { "Win64", "WinARM64" }
filter {}

filter "platforms:Win64"
	architecture "x64"
filter {}

filter "platforms:WinARM64"
	architecture "ARM64"
filter {}

include "src/interp.lua"