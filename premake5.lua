workspace "Projects"
	architecture "x64"
	startproject "passgen"

	flags { "MultiProcessorCompile" }
	configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

function AddProject( Name )
    project (Name)
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++23"
        location (Name)

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        includedirs 
        {
            "%{prj.name}/src"
        }

        files 
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        filter "system:windows"
            systemversion "latest"
        
		filter "configurations:Debug"
            defines { "XY_DEBUG" }
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
            defines { "XY_RELEASE" }
			runtime "Release"
			optimize "on"
			symbols "on"

		filter "configurations:Dist"
            defines { "XY_DIST" }
			runtime "Release"
			symbols "off"
			optimize "on"
   end

group "Apps"
AddProject( "passgen" )
AddProject( "audioplayer" )
AddProject( "autoclicker" )
AddProject( "mousemover" )
AddProject( "containers" )
AddProject( "keylogger" )
AddProject( "filewatcher" )
