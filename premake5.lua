workspace "ModernEngine"
	architecture "x64"
	startproject "Sandbox"
	configurations {"Debug", "Release", "Dist"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "ModernEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "ModernEngine/vendor/GLAD/include"
IncludeDir["ImGui"] = "ModernEngine/vendor/imgui"
IncludeDir["glm"] = "ModernEngine/vendor/glm"
IncludeDir["stb_image"] = "ModernEngine/vendor/stb_image"

group "Dependencies"

include "ModernEngine/vendor/GLFW"
include "ModernEngine/vendor/Glad"
include "ModernEngine/vendor/imgui"

group ""

project "ModernEngine"
	location "ModernEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp", "%{prj.name}/vendor/glm/glm/**.hpp", "%{prj.name}/vendor/glm/glm/**.ini", "%{prj.name}/vendor/stb_image/**.h", "%{prj.name}/vendor/stb_image/**.cpp"}

	defines {"_CRT_SECURE_NO_WARNINGS"}

	includedirs {"%{prj.name}/src", "%{prj.name}/vendor/spdlog/include", "%{IncludeDir.GLFW}", "%{IncludeDir.Glad}", "%{IncludeDir.ImGui}", "%{IncludeDir.glm}", "%{IncludeDir.stb_image}"}

	links { "GLFW", "opengl32.lib", "Glad", "ImGui"}

	pchheader "mnpch.h"
	pchsource "ModernEngine/src/mnpch.cpp"

	filter "system:windows"
		systemversion "latest"

		defines {"MN_PLATFORM_WINDOWS", "MN_BUILD_DLL", "GLFW_INCLUDE_NONE"}

	filter "configurations:Debug" 
		defines "MN_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MN_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MN_DIST"
		buildoptions "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"ModernEngine/vendor/spdlog/include",
		"ModernEngine/src",
		"ModernEngine/vendor",
		"%{IncludeDir.glm}" 
	}

	links
	{
		"ModernEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"MN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MN_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MN_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MN_DIST"
		runtime "Release"
		optimize "on"