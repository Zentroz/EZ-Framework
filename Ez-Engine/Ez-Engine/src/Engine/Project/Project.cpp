#include"Engine/Project/Project.h"
#include"Engine/Core/ErrorHandler.h"

namespace ENGINE {

	bool Project::CreateProject(std::string projectName, fs::path path) {
		fs::path projectDir = path / projectName;
		if (!fs::create_directory(projectDir)) return false;

		m_Directory = projectDir;

		fs::path assetDir = projectDir / "Assets";
		fs::path settingsDir = projectDir / "Settings";
		fs::path libraryDir = projectDir / "Library";

		fs::create_directory(assetDir);
		fs::create_directory(settingsDir);
		fs::create_directory(libraryDir);

		isLoaded = true;

		return true;
	}

	bool Project::OpenProject(fs::path path) {
		if (!fs::is_directory(path / "Settings")) {
			EXCEPTION("Not a project directory!");
			return false;
		}

		m_Directory = path;

		isLoaded = true;
		return true;
	}

}