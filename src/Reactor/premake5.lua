
-- -----------------------------------------------
-- Reactor
-- -----------------------------------------------
project "Reactor"
    kind "ConsoleApp"
    language "C++"
    
    targetdir (bin)
    objdir (binint)
    
    files { 
         "include/**.h", 
         "src/**.cpp" 
    }
    
    defines {
                        -- None
    }

    includedirs{
        "include",
        "%{IncludeDir.Catalyst}/include",
        "%{IncludeDir.spdlog}"
    }
    
    links {
        "Catalyst"
    }
    
    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"
        
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