
-- -----------------------------------------------
-- Catalyst
-- -----------------------------------------------
project "OpenGLRenderer"
    kind "StaticLib"
    language "C++"
	cppdialect "C++latest"
	staticruntime "off"
    
    targetdir (bin)
    objdir (binint)
    
    files {
         "include/**.h",  -- Main files that can be included by clients
         "src/**.h",      -- Core files
         "src/**.c",      -- Core files
         "src/**.cpp",    -- Core files
         "src/**.hpp",    -- Core files
         "src/**.inl",    -- Core files

         "platform/%{cfg.platform}/**.h",      -- Core files
         "platform/%{cfg.platform}/**.c",      -- Core files
         "platform/%{cfg.platform}/**.cpp",    -- Core files
         "platform/%{cfg.platform}/**.hpp",    -- Core files
         "platform/%{cfg.platform}/**.inl"     -- Core files
    }
    
    defines {
        "CATALYST_CORE"
    }

    includedirs{
        "include",
        "%{IncludeDir.Catalyst}/include",
        "%{IncludeDir.Catalyst}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.GLAD}"
    }
    
    libdirs{
    }

    links {
    }
    
    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"
        
    links {
        "opengl32.lib"
    }

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