local imgui_dir = "%{wks.location}/vendor/imgui"

project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   files {
       imgui_dir .. "/*.cpp",
       imgui_dir .. "*.h"
   }

   includedirs {
       imgui_dir,
       imgui_dir .. "/backends"
   }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "Release" }
        optimize "On"
