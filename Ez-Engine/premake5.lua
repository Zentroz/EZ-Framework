workspace "EZ-Framework"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "EDITOR"

	filter "configurations:Debug"
        runtime "Debug"
        staticruntime "off"

    filter "configurations:Release"
        runtime "Release"
        staticruntime "off"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include"Ez-EngineAPI"
include"Ez-Engine"
include"Ez-Editor"
include"GameScripting"

group "Externals"
	include"Ez-Engine/externals/imgui"
	include"Ez-Engine/externals/yaml-cpp"