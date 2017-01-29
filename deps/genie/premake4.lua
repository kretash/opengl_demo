solution "empty_opengl"
	configurations{"debug", "release"}
	location "../../vs2015"
	targetdir "../../bin"
	includedirs "../../include"

	libdirs{ "../../deps/glew" }
	links{ "glew32s" }
	libdirs{ "../../deps/SDL" }
	links{ "SDL2" }
	links{ "SDL2main" }
	links{ "SDL2test" }

	links{ "openGL32" }
		
	language "C++"
	platforms "x64"

	project "demo"
		kind "ConsoleApp"
		files { "../../include/demo/**.h", "../../src/**.cc", "../../src/**.h" }
		files { "../../src/glew/**.c", "../../src/glew/include/**.h" }
		files { "../../include/**/**.cpp", "../../include/**/**.h", "../../include/**/**.hh" }
		files { "../../include/**.h" }

		configuration "Debug"
			targetsuffix "-d" 
			defines { "_CRT_SECURE_NO_WARNINGS", "WIN32","_DEBUG", "DEBUG" }
			flags { "Symbols" }

		configuration "Release"
			defines { "WIN32", "NDEBUG" }
			flags { "Optimize" }
