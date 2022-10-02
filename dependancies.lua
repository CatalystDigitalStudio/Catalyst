VULKAN_SDK = os.getenv("VULKAN_SDK")

    -- Include
IncludeDir = {}
IncludeDir["Catalyst"]  = "%{wks.location}/src/Catalyst/"

IncludeDir["external"]  = "%{wks.location}/external"
IncludeDir["spdlog"]    = "%{wks.location}/external/spdlog/include"
IncludeDir["stb"]       = "%{wks.location}/external/stb"
IncludeDir["zlib"]      = "%{wks.location}/external/zlib"
IncludeDir["vulkan"]    = "%{VULKAN_SDK}/Include"

    -- Library
LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

    -- Renderer
RendererDir = {}
RendererDir["Vulkan"] = "%{wks.location}/renderer/Vulkan"
RendererDir["DX11"] = "%{wks.location}/renderer/DX11"
RendererDir["DX12"] = "%{wks.location}/renderer/DX12"