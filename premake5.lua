workspace "MandelbrotRenderer"
	architecture "x86_64"
	startproject "MandelbrotRenderer"

	configurations { "Debug", "Release" }

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "GLFW"
	include "GLAD"
group ""

group "Core"
	include "MandelbrotRenderer"
group ""
