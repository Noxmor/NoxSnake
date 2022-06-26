project "NoxSnake"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp"
	}

	includedirs
	{
		"src"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

		filter "configurations:Debug"
		defines "NS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "NS_DIST"
		runtime "Release"
		optimize "on"