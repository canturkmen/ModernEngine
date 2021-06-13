workspace "ModernEngine"
	architecture "x64"
	configurations {"Debug", "Release", "Dist"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "ModernEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "ModernEngine/vendor/GLAD/include"
IncludeDir["ImGui"] = "ModernEngine/vendor/imgui"

include "ModernEngine/vendor/GLFW"
include "ModernEngine/vendor/Glad"
include "ModernEngine/vendor/imgui"

project "ModernEngine"
	location "ModernEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"}

	includedirs {"%{prj.name}/src", "%{prj.name}/vendor/spdlog/include", "%{IncludeDir.GLFW}", "%{IncludeDir.Glad}", "%{IncludeDir.ImGui}"}

	links { "GLFW", "opengl32.lib", "Glad", "ImGui"}

	pchheader "mnpch.h"
	pchsource "ModernEngine/src/mnpch.cpp"

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		defines {"MN_PLATFORM_WINDOWS", "MN_BUILD_DLL", "GLFW_INCLUDE_NONE"}

		postbuildcommands {("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")}

	filter "configurations:Debug" 
		defines "MN_DEBUG"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "MN_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "MN_DIST"
		buildoptions "/MD"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"ModernEngine/src"
	}

	links
	{
		"ModernEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MN_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "MN_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "MN_DIST"
		buildoptions "/MD"
		optimize "On"