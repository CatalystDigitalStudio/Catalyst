include "dependancies.lua"

workspace "Catalyst"
    architecture"x64"
    startproject "Reactor"

    configurations { "Debug", "Release", "Distro" }

	flags { "MultiProcessorCompile" }

bin    = "../build/bin/%{cfg.buildcfg}-%{cfg.architecture}/%{prj.name}"
binint = "../build/bin/int/%{cfg.buildcfg}-%{cfg.architecture}/%{prj.name}"

group "Dependencies"
    --None yet
group ""

include "src/Catalyst"
include "src/Reactor"