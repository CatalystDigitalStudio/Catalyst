workspace "Catalyst"
    architecture"x64"
    configurations { "Debug", "Release", "Distro" }

    location "src"
    startproject "Reaction"

bin = "../build/bin/%{cfg.buildcfg}-%{cfg.architecture}/%{prj.name}"
binint = "../build/bin/int/%{cfg.buildcfg}-%{cfg.architecture}/%{prj.name}"

project "Catalyst"
    location "src/Catalyst"
    kind "StaticLib"
    language "C++"
    
    targetdir (bin)
    objdir (binint)
    
    pchheader "phc.h"
    pchsource "Catalyst/src/pch.cpp"
    
    files {
         "%{wks.location}/Catalyst/include/**.h",  -- Main files that can be included by clients
         "%{wks.location}/Catalyst/src/**.h",      -- Core files
         "%{wks.location}/Catalyst/src/**.c",      -- Core files
         "%{wks.location}/Catalyst/src/**.cpp",    -- Core files
         "%{wks.location}/Catalyst/src/**.hpp",    -- Core files
         "%{wks.location}/Catalyst/src/**.inl"     -- Core files
    }
    
    defines {
                        -- None
    }

    includedirs{
        "%{wks.location}/Catalyst/src/"
    }
    
    links {
                        -- None
    }
    
    filter "system:windows"
        cppdialect "C++20"
        staticruntime "on"
        systemversion "latest"

    filter "configurations:Debug"
       defines { "CTYL_DEBUG", "DEBUG" }
       symbols "On"
    
    filter "configurations:Release"
       defines { "CTYL_NDEBUG", "NDEBUG" }
       optimize "On"

-- -----------------------------------------------
-- Reactor
-- -----------------------------------------------
project "Reactor"
    location "src/Reactor"
    kind "ConsoleApp"
    language "C++"
    
    targetdir (bin)
    objdir (binint)
    
    files { 
         "%{wks.location}/Reactor/include/**.h", 
         "%{wks.location}/Reactor/src/**.cpp" 
    }
    
    defines {
                        -- None
    }

    includedirs{
        "%{wks.location}/Reactor/include",
        "%{wks.location}/Catalyst/include"
    }
    
    links {
        "Catalyst"
    }
    
    filter "system:windows"
        cppdialect "C++20"
        staticruntime "on"
        systemversion "latest"

    filter "configurations:Debug"
       defines { "CTYL_DEBUG", "DEBUG" }
       symbols "On"
    
    filter "configurations:Release"
       defines { "CTYL_NDEBUG", "NDEBUG" }
       optimize "On"