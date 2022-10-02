
-- -----------------------------------------------
-- zlib
-- -----------------------------------------------
project "zlib"
    kind "StaticLib"
    language "C"
	staticruntime "off"
    
    targetdir (bin)
    objdir (binint)
    
    files {
         "**.h",      -- Core files
         "**.c"       -- Core files
    }
    
    defines {

    }

    includedirs{
        "%{IncludeDir.zlib}"
    }