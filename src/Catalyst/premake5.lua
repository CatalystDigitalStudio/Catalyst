
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
    
    pchheader "phc.h"
    pchsource "src/pch.cpp"
    
    files {
         "include/**.h",  -- Main files that can be included by clients
         "src/**.h",      -- Core files
         "src/**.c",      -- Core files
         "src/**.cpp",    -- Core files
         "src/**.hpp",    -- Core files
         "src/**.inl"     -- Core files
    }
    
    defines {
        "STBTT_STATIC"
    }

    includedirs{
        "%{IncludeDir.Catalyst}/src",
        "%{IncludeDir.spdlog}"
    }
    
    links {
                        -- None
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