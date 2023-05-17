project "MandelbrotRenderer"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/*.cpp",
		"include/*.h"
	}

	includedirs {
		"%{wks.location}/GLAD/include",
		"%{wks.location}/GLFW/include"
	}

	links {
		"GLFW",
		"GLAD",

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
