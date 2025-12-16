project "GameScripting"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    files { "**.h", "**.cpp" }

    includedirs {
        "%{wks.location}/Ez-EngineAPI/src",
    }