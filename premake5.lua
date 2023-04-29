workspace "MemoryManager"
    configurations { "Debug", "Release" }
    platforms { "Win32", "Win64"}
    startproject "Test"

    includedirs { "Library/include" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"

    filter "platforms:Win32"
        system "Windows"
        architecture "x86"

    filter "platforms:Win64"
        system "Windows"
        architecture "x86_64"    

    project "MemoryManager"
        kind "StaticLib"
        language "C++"
        location "Library"

        files { "Library/**.h", "Library/**.cc" }

    project "Test"
        kind "ConsoleApp"
        language "C++"
        location "Test"
        dependson { "MemoryManager" }

        files { "Test/**.cpp" }

        libdirs { "Library/bin/%{cfg.buildcfg}/%{cfg.platform}" }
        links { "MemoryManager" }