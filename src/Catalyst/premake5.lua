
-- -----------------------------------------------
-- Catalyst
-- -----------------------------------------------
project "Catalyst"
    kind "StaticLib"
    language "C++"
	cppdialect "C++latest"
	staticruntime "off"
    
    targetdir (bin)
    objdir (binint)
    
    pchheader "pch.h"
    pchsource "src/pch.cpp"
    
    files {
         "include/**.h",                       -- Main files that can be included by clients

         "src/**.h",                           -- Core files
         "src/**.c",                           -- Core files
         "src/**.cpp",                         -- Core files
         "src/**.hpp",                         -- Core files
         "src/**.inl",                         -- Core files

         "platform/Platform.h",                -- Core platform files
         "platform/Platform.cpp",              -- Core platform files
         "platform/%{cfg.platform}/**.h",      -- Core platform files
         "platform/%{cfg.platform}/**.c",      -- Core platform files
         "platform/%{cfg.platform}/**.cpp",    -- Core platform files
         "platform/%{cfg.platform}/**.hpp",    -- Core platform files
         "platform/%{cfg.platform}/**.inl"     -- Core platform files
    }
    
    defines {
        "STBTT_STATIC",
        "CATALYST_CORE"
    }

    includedirs{
        "%{IncludeDir.Catalyst}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.external}"
    }
    
    links {
        "zlib",
        "VulkanRenderer"
    }
    
    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

    filter "configurations:Debug"
       defines { "CATALYST_DEBUG", "DEBUG" }
       runtime "Debug"
       symbols "On"
    
    filter "configurations:Release"
       defines { "CATALYST_RELEASE", "CATALYST_NDEBUG", "NDEBUG" }
       runtime "Release"
       optimize "On"

    filter "configurations:Distro"
       defines { "CATALYST_DISTRO", "CATALYST_NDEBUG", "NDEBUG" }
       runtime "Release"
       optimize "On"