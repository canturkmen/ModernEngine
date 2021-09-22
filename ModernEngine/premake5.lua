project "ModernEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "mnpch.h"
	pchsource "src/mnpch.cpp"

	files {"src/**.h", "src/**.cpp", "vendor/glm/glm/**.hpp", "vendor/glm/glm/**.ini", "vendor/stb_image/**.h", "vendor/stb_image/**.cpp", "vendor/ImGuizmo/ImGuizmo.h", "vendor/ImGuizmo/ImGuizmo.cpp"}

	defines {"_CRT_SECURE_NO_WARNINGS"}

	includedirs {"src", "vendor/spdlog/include", "%{IncludeDir.GLFW}", "%{IncludeDir.Glad}", "%{IncludeDir.ImGui}", "%{IncludeDir.glm}", "%{IncludeDir.stb_image}", "%{IncludeDir.entt}", 
				"%{IncludeDir.yaml_cpp}", 
				"%{IncludeDir.ImGuizmo}",
				"%{IncludeDir.Box2D}"
				}

	links { "GLFW", "opengl32.lib", "Glad", "ImGui", "yaml-cpp", "Box2D"}

	filter "files:vendor/ImGuizmo/**.cpp"
		flags{"NoPCH"}	

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