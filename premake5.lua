include "./vendor/premake/premake-customization/solution_items.lua"
include "Dependencies.lua"

workspace "ModernEngine"
	architecture "x86_64"
	startproject "ModernEngine-Editor"
	configurations {"Debug", "Release", "Dist"}
	solution_items {".editorconfig"}
	flags {"MultiProcessorCompile"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/ModernEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/ModernEngine/vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/ModernEngine/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/ModernEngine/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/ModernEngine/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/ModernEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/ModernEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/ModernEngine/vendor/ImGuizmo"
IncludeDir["Box2D"] = "%{wks.location}/ModernEngine/vendor/Box2D/include"

group "Dependencies"
include "vendor/premake"
include "ModernEngine/vendor/GLFW"
include "ModernEngine/vendor/Glad"
include "ModernEngine/vendor/imgui"
include "ModernEngine/vendor/yaml-cpp"
include "ModernEngine/vendor/Box2D"
group ""

include "ModernEngine"
include "ModernEngine-Editor"
include "Sandbox"