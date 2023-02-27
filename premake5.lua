include "./vendor/premake/premake-customization/solution_items.lua"
include "Dependencies.lua"

workspace "ModernEngine"
	architecture "x86_64"
	startproject "ModernEngine-Editor"

	configurations 
	{
		"Debug", 
		"Release", 
		"Dist"
	}

	solution_items 
	{
		".editorconfig"
	}

	flags 
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "ModernEngine/vendor/Box2D"
	include "ModernEngine/vendor/GLFW"
	include "ModernEngine/vendor/Glad"
	include "ModernEngine/vendor/msdf-atlas-gen"
	include "ModernEngine/vendor/imgui"
	include "ModernEngine/vendor/yaml-cpp"
group ""

group "Core"
	include "ModernEngine"
	include "ModernEngine-ScriptCore"
group ""

group "Misc"
	include "Sandbox"
group ""

group "Tools"
	include "ModernEngine-Editor"
group ""

