VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["Catalyst"]  = "%{wks.location}/src/Catalyst/"
IncludeDir["spdlog"]    = "%{wks.location}/src/Catalyst/external/spdlog/include"
IncludeDir["stb_image"] = "%{wks.location}/src/Catalyst/external/stb_image"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"