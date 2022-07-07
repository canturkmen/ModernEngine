include "./vendor/premake/premake-customization/solution_items.lua"
include "Dependencies.lua"

workspace "ModernEngine"
	architecture "x86_64"
	startproject "ModernEngine-Editor"
	configurations {"Debug", "Release", "Dist"}
	solution_items {".editorconfig"}
	flags {"MultiProcessorCompile"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "ModernEngine/vendor/GLFW"
	include "ModernEngine/vendor/Glad"
	include "ModernEngine/vendor/imgui"
	include "ModernEngine/vendor/yaml-cpp"
	include "ModernEngine/vendor/Box2D"
group ""

include "ModernEngine"
include "Sandbox"
include "ModernEngine-Editor"