VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/ModernEngine/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/ModernEngine/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/ModernEngine/vendor/Box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/ModernEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/ModernEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/ModernEngine/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/ModernEngine/vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/ModernEngine/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/ModernEngine/vendor/entt/include"
IncludeDir["mono"] = "%{wks.location}/ModernEngine/vendor/mono/include" 
IncludeDir["shaderc"] = "%{wks.location}/ModernEngine/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/ModernEngine/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["mono"] = "%{wks.location}/ModernEngine/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}

Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"