workspace "NoxSnake"
	architecture "x86_64"
	startproject "NoxSnake"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "NoxSnake"

if(os.isdir("Sandbox")) then
	include "Sandbox"
end