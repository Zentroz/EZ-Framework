project "Bullet"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/build/bin/%{cfg.buildcfg}")
    objdir ("%{wks.location}/build/bin-int/%{cfg.buildcfg}")

    files {
        "include/**.h",
        "include/**.cpp"
    }

    includedirs {
        "include",
    }

    libdirs {
        "lib"
    }
