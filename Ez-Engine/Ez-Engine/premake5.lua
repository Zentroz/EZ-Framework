project "Ez-Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "externals/bullet/include",
        "externals/mono/include",
        "externals/imgui/include",
        "%{wks.location}/packages/directxtk_uwp.2025.10.28.2/include"
    }

    libdirs {
        "externals/bullet/lib",
        "externals/mono/lib",
    }

    links {
        "mono-2.0-sgen",
        "ImGui"
    }

    filter "configurations:Debug"
        runtime "Debug"        -- MDd
        staticruntime "off"

        libdirs {
            "externals/dxtk/lib/Debug"
        }

        links {
            "BulletDynamics_Debug",
            "BulletCollision_Debug",
            "LinearMath_Debug",
            "DirectXTK"
        }

    filter "configurations:Release"
        runtime "Release"      -- MD
        staticruntime "off"

        libdirs {
            "externals/dxtk/lib/Release"
        }

        links {
            "BulletDynamics",
            "BulletCollision",
            "LinearMath",
            "DirectXTK"
        }