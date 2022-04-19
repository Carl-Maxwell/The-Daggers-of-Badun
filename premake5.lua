workspace "the_daggers_of_badun"
	architecture "x64"

	configurations
	{
		"debug",
		"release",
		"dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
-- IncludeDir["GLFW"]  = "SauropodEngine/vendor/glfw/include"
-- IncludeDir["Glad"]  = "SauropodEngine/vendor/Glad/include"
-- IncludeDir["imgui"] = "SauropodEngine/vendor/imgui/"

-- include "SauropodEngine/vendor/glfw"
-- include "SauropodEngine/vendor/Glad"
-- include "SauropodEngine/vendor/imgui"

project "badun_engine"
	location "badun_engine"
	kind "ConsoleApp"
	language "C++"

	-- TODO: for some reason I wasn't able to use %{prj.location}

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}") 

	-- pchheader "hzpch.h"
	-- pchsource "%{prj.name}/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
	}
	links 
	{ 

	}

	filter "system:windows"
		cppdialect "C++17"
		-- staticruntime "Off"
		systemversion "latest"

		-- defines 
		-- {
		
		-- }
		
		-- postbuildcommands
		-- {
		-- 	("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/badun_sandbox"),
		-- 	("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/badun_game")
		-- }

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		buildoptions "/MDd" -- runtime library: multi-threaded debug DLL
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		buildoptions "/MD" -- runtime library: multi-threaded DLL
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		buildoptions "/MD" -- runtime library: multi-threaded DLL
		optimize "On"
	
project "badun_sandbox"
	location "badun_sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}") 

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"badun_engine/vendor/spdlog/include",
		"badun_engine/src"
	}

	links
	{
		"badun_engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		-- staticruntime "On"
		systemversion "latest"

	-- defines 
	-- {
	-- 	"HZ_PLATFORM_WINDOWS",
	-- 	"_WINDLL"
	-- }

	filter "configurations:Debug"
		-- defines "HZ_DEBUG"
		buildoptions "/MDd" -- runtime library: multi-threaded debug DLL
		symbols "On"

	filter "configurations:Release"
		-- defines "HZ_RELEASE"
		buildoptions "/MD" -- runtime library: multi-threaded DLL
		optimize "On"

	filter "configurations:Dist"
		-- defines "HZ_DIST"
		buildoptions "/MD" -- runtime library: multi-threaded DLL
		optimize "On"


project "badun_game"
	location "badun_game"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}") 

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"badun_engine/vendor/spdlog/include",
		"badun_engine/src"
	}

	links
	{
		"badun_engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		-- staticruntime "On"
		systemversion "latest"

	-- defines 
	-- {
	-- 	"HZ_PLATFORM_WINDOWS",
	-- 	"_WINDLL"
	-- }

	filter "configurations:Debug"
		-- defines "HZ_DEBUG"
		buildoptions "/MDd" -- runtime library: multi-threaded debug DLL
		symbols "On"

	filter "configurations:Release"
		-- defines "HZ_RELEASE"
		buildoptions "/MD" -- runtime library: multi-threaded DLL
		optimize "On"

	filter "configurations:Dist"
		-- defines "HZ_DIST"
		buildoptions "/MD" -- runtime library: multi-threaded DLL
		optimize "On"
