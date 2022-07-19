project "ModernEngine-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/ModernEngine/vendor/spdlog/include",
		"%{wks.location}/ModernEngine/src",
		"%{wks.location}/ModernEngine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
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