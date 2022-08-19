local ModernEngineRootDirectory = "../../../.."
include (ModernEngineRootDirectory .. "/vendor/premake/premake-customization/solution_items.lua")

workspace "Sanbdox"
	architecture "x86_64"
	startproject "ModernEngine-Editor"

	configurations 
	{
		"Debug", 
		"Release", 
		"Dist"
	}

	flags 
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "Sandbox"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Binaries")
	objdir ("Intermediates")

	files 
	{
		"Source/**.cs",
		"Properties/**.cs"
	}

	links 
	{
		"ModernEngine-ScriptCore",
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

group "ModernEngine"
	include (ModernEngineRootDirectory .. "/ModernEngine-ScriptCore")
group ""