include "dependancies.lua"

workspace "Catalyst"
    architecture"x64"
    startproject "Reactor"

    configurations { "Debug", "Release", "Distro" }

	flags { "MultiProcessorCompile" }

bin    = "%{wks.location}/build/bin/%{cfg.buildcfg}-%{cfg.architecture}"
binint = "%{wks.location}/build/bin/int/%{cfg.buildcfg}-%{cfg.architecture}/%{prj.name}"

group "Dependencies"
    include "external/zlib"
group "Renderers"
    include "src/renderer/Vulkan"
    include "src/renderer/DX11"
group ""

include "src/Catalyst"
include "src/Reactor"