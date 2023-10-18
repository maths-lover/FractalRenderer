local imgui_directory = "%{wks.location}/vendor/imgui"

project "FractalRenderer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/*.cpp"
		-- "include/*.h"
	}
	files {
		-- "%{wks.location}/vendor/imgui/**.cpp",
		-- "%{wks.location}/vendor/imgui/backends/**.cpp"
		imgui_directory .. "/imgui.cpp",
		imgui_directory .. "/backends/imgui_impl_glfw.cpp",
		imgui_directory .. "/backends/imgui_impl_opengl3.cpp",
		imgui_directory .. "/backends/imgui_impl_opengl3_loader.h"
	}

	includedirs {
		"%{wks.location}/vendor/GLAD/include",
		"%{wks.location}/vendor/GLFW/include",
		"%{wks.location}/vendor/imgui",
		"%{wks.location}/vendor/imgui/backends",
		"%{prj.location}/include"
	}

	links {
		"GLFW",
		"GLAD",

		"imgui",

		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		linkoptions {
			"/NODEFAULTLIB:LIBCMT",
			"/NODEFAULTLIB:LIBCMTD"
		}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
