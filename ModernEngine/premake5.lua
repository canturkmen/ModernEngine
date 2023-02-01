project "ModernEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "mnpch.h"
	pchsource "src/mnpch.cpp"

	files  
	{
		"src/**.h", "src/**.cpp", 
		"vendor/glm/glm/**.hpp", 
		"vendor/glm/glm/**.ini", 
		"vendor/stb_image/**.h", 
		"vendor/stb_image/**.cpp", 
		"vendor/ImGuizmo/ImGuizmo.h", 
		"vendor/ImGuizmo/ImGuizmo.cpp",
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS", 
		"GLFW_INCLUDE_NONE"
	}

	includedirs 
	{
		"src", 
		"vendor/spdlog/include", 
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.GLFW}", 
		"%{IncludeDir.Glad}", 
		"%{IncludeDir.ImGui}", 
		"%{IncludeDir.glm}", 
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}", 
		"%{IncludeDir.mono}",
		"%{IncludeDir.yaml_cpp}", 
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.filewatch}",
		"%{IncludeDir.VulkanSDK}"
	}

	links 
	{ 
		"Box2D", 
		"GLFW", 
		"Glad", 
		"ImGui", 
		"yaml-cpp", 
		"opengl32.lib", 
		"%{Library.mono}",
	}

	filter "files:vendor/ImGuizmo/**.cpp"
		flags{"NoPCH"}	

	filter "system:windows"
		systemversion "latest"

		defines {"MN_PLATFORM_WINDOWS", "MN_BUILD_DLL"}

		links 
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}",
		}

	filter "configurations:Debug" 
		defines "MN_DEBUG"
		runtime "Debug"
		symbols "on"

		links 
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "MN_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "MN_DIST"
		buildoptions "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}