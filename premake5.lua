workspace "ModernEngine"
	architecture "x64"
	configurations {"Debug", "Release", "Dist"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ModernEngine"
	location "ModernEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "mnpch.h"
	pchsource "ModernEngine/src/mnpch.cpp"

	files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"}

	includedirs {"%{prj.name}/vendor/spdlog/include"}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "10.0"

		defines {"MN_PLATFORM_WINDOWS", "MN_BUILD_DLL"}

		postbuildcommands {("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")}

	filter "configurations:Debug" 
		defines "MN_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "MN_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "MN_DIST"
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
		systemversion "10.0"

		defines
		{
			"MN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MN_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "MN_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "MN_DIST"
		optimize "On"