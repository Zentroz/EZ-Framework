project "Ez-Editor"
    kind "WindowedApp"
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
        "%{wks.location}/Ez-Engine/src",
        "%{wks.location}/Ez-Engine/externals/imgui/include",
        "%{wks.location}/Ez-Engine/externals/mono/include"
    }

    libdirs {
        "%{wks.location}/Ez-Engine/externals/mono/lib"
    }

    links {
        "Ez-Engine",
        "mono-2.0-sgen",
        "ImGui"
    }

    filter "configurations:Debug"
        runtime "Debug"        -- MDd
        staticruntime "off"

    filter "configurations:Release"
        runtime "Release"      -- MD
        staticruntime "off"