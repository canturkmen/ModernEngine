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

group "Dependencies"

include "ModernEngine/vendor/GLFW"
include "ModernEngine/vendor/Glad"
include "ModernEngine/vendor/imgui"

group ""

project "ModernEngine"
	location "ModernEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp", "%{prj.name}/vendor/glm/glm/**.hpp", "%{prj.name}/vendor/glm/glm/**.ini"}

	includedirs {"%{prj.name}/src", "%{prj.name}/vendor/spdlog/include", "%{IncludeDir.GLFW}", "%{IncludeDir.Glad}", "%{IncludeDir.ImGui}", "%{IncludeDir.glm}"}

	links { "GLFW", "opengl32.lib", "Glad", "ImGui"}

	pchheader "mnpch.h"
	pchsource "ModernEngine/src/mnpch.cpp"

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {"MN_PLATFORM_WINDOWS", "MN_BUILD_DLL", "GLFW_INCLUDE_NONE"}

		postbuildcommands {("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")}

	filter "configurations:Debug" 
		defines "MN_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MN_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MN_DIST"
		buildoptions "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		"%{IncludeDir.glm}" 
	}

	links
	{
		"ModernEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"MN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MN_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MN_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MN_DIST"
		runtime "Release"
		optimize "On"