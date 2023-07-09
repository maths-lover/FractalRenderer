workspace "FractalRenderer"
	architecture "x86_64"
	startproject "FractalRenderer"

	configurations { "Debug", "Release" }

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/GLFW"
	include "vendor/GLAD"
	include "vendor/imgui"
group ""

group "Core"
	include "FractalRenderer"
group ""
